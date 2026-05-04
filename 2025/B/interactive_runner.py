#!/usr/bin/env python3

"""
Interactive runner wrapper for BOCA run/<lang> scripts.

This script replaces regular BOCA runners and is modified per-problem and per-language
to embed the original BOCA run/<lang> script content and a base64-encoded interactor.exe

It:
  - materializes interactor.exe and the original runner (as real_runner.sh)
  - creates named pipes stdin0/stdout0
  - launches interactor and solution runner under safeexec with proper wiring
  - enforces nruns == 1 for interactive problems
  - returns the interactor verdict on stdout when successful
"""

import base64
import errno
import os
import signal
import subprocess as sp
import sys
import time
from typing import Optional


def which(cmd: str) -> Optional[str]:
    from shutil import which as _which
    return _which(cmd) or (os.path.exists(cmd) and os.access(cmd, os.X_OK) and cmd) or None


def write_file(path: str, data: bytes, mode: int = 0o600) -> None:
    with open(path, 'wb') as f:
        f.write(data)
    os.chmod(path, mode)


def write_text(path: str, text: str, mode: int = 0o755) -> None:
    with open(path, 'w', encoding='utf-8') as f:
        f.write(text)
    os.chmod(path, mode)


def decode_interactor(target: str) -> None:
    b64 = "__INTERACTOR_B64__"
    data = base64.b64decode(b64)
    write_file(target, data, 0o700)


def materialize_runner(target: str) -> None:
    b64 = "__RUNNER_B64__"
    data = base64.b64decode(b64)
    write_file(target, data, 0o755)


def mkfifo_safe(path: str, mode: int = 0o600) -> None:
    try:
        os.unlink(path)
    except (OSError, FileNotFoundError):
        pass
    os.mkfifo(path, mode)
    try:
        os.chmod(path, mode)
    except Exception:
        pass


def setsid_preexec():
    os.setsid()


def interactor_limits_preexec(cpu_seconds: int, mem_kb: int, nproc: int):
    def _preexec():
        try:
            os.setsid()
        except Exception:
            pass
        # Apply best-effort resource limits on POSIX
        try:
            import resource as res
            # CPU time limit
            try:
                res.setrlimit(res.RLIMIT_CPU, (cpu_seconds, cpu_seconds))
            except Exception:
                pass
            # Address space limit (bytes)
            try:
                mem_bytes = mem_kb * 1024
                res.setrlimit(res.RLIMIT_AS, (mem_bytes, mem_bytes))
            except Exception:
                pass
            # Data segment limit (bytes)
            try:
                res.setrlimit(res.RLIMIT_DATA, (mem_bytes, mem_bytes))
            except Exception:
                pass
            # No core dumps
            try:
                res.setrlimit(res.RLIMIT_CORE, (0, 0))
            except Exception:
                pass
            # Open files limit
            try:
                res.setrlimit(res.RLIMIT_NOFILE, (10, 10))
            except Exception:
                pass
            # Process/threads limit
            try:
                res.setrlimit(res.RLIMIT_NPROC, (nproc, nproc))
            except Exception:
                pass
        except Exception:
            pass
    return _preexec


def kill_pgid(pgid: int) -> None:
    try:
        os.killpg(pgid, signal.SIGTERM)
        time.sleep(0.2)
        os.killpg(pgid, signal.SIGKILL)
    except Exception:
        pass


def main(argv: list[str]) -> int:
    if len(argv) < 4:
        print("interactive runner: parameter problem, not enough args", file=sys.stdout)
        return 43

    submission_exe = argv[1]
    input_file = argv[2]
    timelimit_s = argv[3]
    nruns = argv[4] if len(argv) >= 5 else None
    mem_mb = argv[5] if len(argv) >= 6 else None
    out_kb = argv[6] if len(argv) >= 7 else None
    # TODO - should we have a default big output limit for interactive problems?

    if nruns and int(nruns) != 1:
        print(f"interactive runner: nruns must be 1 (got {nruns})", file=sys.stdout)
        return 5
    nruns = '1'

    if not submission_exe or not input_file or not timelimit_s:
        print("interactive runner: parameter problem", file=sys.stdout)
        return 43
    if not os.path.exists(input_file) or not os.access(input_file, os.R_OK):
        print(f"interactive runner: {input_file} not found (or is not in the current dir) or it's not readable", file=sys.stdout)
        return 45

    sf = which('safeexec') or '/usr/bin/safeexec'
    if not os.path.exists(sf) or not os.access(sf, os.X_OK):
        print(f"interactive runner: {sf} not found or it's not executable", file=sys.stdout)
        return 46

    # Materialize artifacts
    interactor_exe = './interactor.exe'
    decode_interactor(interactor_exe)

    runner_path = './real_runner.sh'
    materialize_runner(runner_path)

    # solution runners use safeexec with -i stdin0 -o stdout0, we pre-create those names
    # as FIFOs to hot-swap the streams and plug then in the interactor
    sol_in = 'stdin0'   # interactor -> solution stdin
    sol_out = 'stdout0' # solution -> interactor stdout
    interactor_output = os.path.abspath('interactor.output')
    interactor_stderr = 'interactor.stderr'
    runner_stdout = 'runner.stdout'
    runner_stderr = 'runner.stderr'
    mkfifo_safe(sol_in, 0o666)
    mkfifo_safe(sol_out, 0o666)
    write_file(interactor_output, b'', 0o600)
    write_file(interactor_stderr, b'', 0o600)
    write_file(runner_stdout, b'', 0o600)
    write_file(runner_stderr, b'', 0o600)

    # Prevent FIFO open() deadlocks by holding temporary RDWR descriptors.
    # This ensures a reader and a writer exist so specific RB/WB opens don't block.
    try:
        ka_in = os.open(sol_in, os.O_RDWR | os.O_NONBLOCK)
    except Exception:
        ka_in = None
    try:
        ka_out = os.open(sol_out, os.O_RDWR | os.O_NONBLOCK)
    except Exception:
        ka_out = None

    # Interactor limits
    INT_CPU_PAD = 5
    INT_WALL_PAD = 25
    INT_MEM_MB = 512
    INT_NPROC = 1
    INT_CPU = int(timelimit_s) + INT_CPU_PAD
    INT_WALL = INT_CPU + INT_WALL_PAD
    # Best-effort wallclock limit (watchdog handled in main loop)
    INT_MEM_KB = INT_MEM_MB * 1000

    # Launch interactor directly (no safeexec) with resource limits, wired to FIFOs
    inter_preexec = interactor_limits_preexec(INT_CPU, INT_MEM_KB, INT_NPROC)
    with open(sol_out, 'rb', buffering=0) as inter_stdin, \
            open(sol_in, 'wb', buffering=0) as inter_stdout, \
            open(interactor_stderr, 'ab', buffering=0) as inter_stderr:
        interactor_cmd = [interactor_exe, input_file, interactor_output]
        print("interactive runner: launching interactor:", ' '.join(interactor_cmd), file=sys.stderr,)
        interactor = sp.Popen(
            interactor_cmd,
            stdin=inter_stdin,
            stdout=inter_stdout,
            stderr=inter_stderr,
            preexec_fn=inter_preexec,
        )
    interactor_pgid = interactor.pid

    runner_cmd = [runner_path, submission_exe, '/dev/null', timelimit_s]
    if nruns:
        runner_cmd.append(nruns)
    if mem_mb:
        runner_cmd.append(mem_mb)
    if out_kb:
        runner_cmd.append(out_kb)

    print("interactive runner: launching solution runner:", ' '.join(runner_cmd), file=sys.stderr)
    with open(runner_stderr, 'ab', buffering=0) as sol_runner_stderr, \
            open(runner_stdout, 'ab', buffering=0) as sol_runner_stdout:
        solution = sp.Popen(
            runner_cmd,
            preexec_fn=setsid_preexec,
            stderr=sol_runner_stderr,
            stdout=sol_runner_stdout,
        )
    solution_pgid = solution.pid

    # Brief grace period so safeexec/solution opens the FIFOs before we drop keepalives.
    # This avoids the interactor seeing EOF on stdout0 if the solution is still starting.
    try:
        time.sleep(0.5)
    except Exception:
        pass
    # Close keepalive descriptors now that both processes are started.
    try:
        if ka_in is not None:
            os.close(ka_in)
    except Exception:
        pass
    try:
        if ka_out is not None:
            os.close(ka_out)
    except Exception:
        pass

    # --- Wait for first exit; enforce best-effort wallclock; if first failed, kill peer ---
    sol_ec = None
    int_ec = None
    wall_deadline = time.monotonic() + INT_WALL
    while True:
        # Wallclock watchdog: if exceeded, terminate both sides
        if time.monotonic() >= wall_deadline:
            try:
                print(f"interactive runner: wallclock exceeded (~{INT_WALL}s), terminating", file=sys.stderr)
            except Exception:
                pass
            kill_pgid(interactor_pgid)
            kill_pgid(solution_pgid)
            # Give the OS a brief moment to deliver signals
            time.sleep(0.1)
            break
        if sol_ec is None:
            sol_ec = solution.poll()
        if int_ec is None:
            int_ec = interactor.poll()
        if sol_ec is not None:
            if sol_ec != 0:
                kill_pgid(interactor_pgid)
            # wait to collect
            try:
                int_ec = interactor.wait(timeout=2)
            except Exception:
                pass
            break
        if int_ec is not None:
            # If unexpected interactor failure, stop solution
            if int_ec not in (4, 6, -9, -15):
                kill_pgid(solution_pgid)
            try:
                sol_ec = solution.wait(timeout=2)
            except Exception:
                pass
            break
        time.sleep(0.05)

    # Collect final return codes if still None
    if sol_ec is None:
        sol_ec = solution.wait()
    if int_ec is None:
        int_ec = interactor.wait()

    for p in (sol_in, sol_out):
        try:
            os.unlink(p)
        except Exception:
            pass

    # Bundle logs
    try:
        def include_logs(logfile, desc):
            print(f"!! << {desc} >> !!", file=sys.stderr)
            try:
                with open(logfile, 'rb') as f:
                    data = f.read()
                    if data:
                        sys.stderr.buffer.write(data)
            except Exception:
                print(f"  ... failed to include logs", file=sys.stderr)
                pass
            print(f"¡¡ >> {desc} << ¡¡", file=sys.stderr)
        include_logs(runner_stderr, 'solution runner stder')
        include_logs(interactor_stderr, 'interactor stder')
        print(f"interactor pid {interactor.pid}, exitcode {int_ec}", file=sys.stderr)
        print(f"solution   pid {solution.pid}, exitcode {sol_ec}", file=sys.stderr)
    except Exception:
        pass

    # Verdict policy
    # Interactor failure => judge/internal error (4)
    if int_ec not in (4, 6, -9, -15):
        print(f"Interactor unexpected exit code: {int_ec}")
        return 4

    # Nonzero solution exit code when interactor didn't fail => report runner result
    # Lets WA fallthrough to be handled down the line
    if sol_ec != 0 and int_ec < 0:
        with open(runner_stdout, 'rb') as f:
            data = f.read()
        if data:
            sys.stdout.buffer.write(data)
            sys.stdout.flush()
        else:
            print(f"interactive runner: no details on solution failure. if interactor (ec={int_ec}) reported something we'll include it below:")
            with open(interactor_output , 'rb') as f:
                data = f.read()
                if data:
                    sys.stdout.buffer.write(data)
                    sys.stdout.flush()
        return int(sol_ec)

    # If interactor finished cleanly with verdict AC/WA, print its verdict
    if int_ec in (4, 6):
        try:
            with open(interactor_output , 'rb') as f:
                data = f.read()
            if not data:
                try:
                    with open(interactor_stderr, 'rb') as errf:
                        errors = errf.read().splitlines()
                        if errors:
                            for line in reversed(errors):
                                if line.strip():
                                    data = line
                                    break
                except Exception:
                    pass

            if not data:
                print(f"Interactor exited {int_ec} but produced no verdict output")
                return 4
            sys.stdout.buffer.write(data)
            sys.stdout.flush()
            return 0
        except Exception:
            print("Interactor verdict output failure")
            return 4

    # SIGPIPE/SIGTERM acceptable only if solution failed; with SOL_EC==0 it's unexpected
    if int_ec in (-9, -15):
        print(f"Interactor failed ({int_ec}) but solution exited 0")
        return 4

    print(f"Unhandled outcome for interactive runner (INT_EC={int_ec}, SOL_EC={sol_ec})")
    return 4


if __name__ == '__main__':
    try:
        sys.exit(main(sys.argv))
    except KeyboardInterrupt:
        sys.exit(4)
