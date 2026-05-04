import os
import subprocess
import shutil

def is_tool(name):
    return shutil.which(name) is not None

def create_makefile(problem_path, problem_name):
    answers_dir = os.path.join(problem_path, "answers")
    
    if not os.path.exists(answers_dir):
        return None
    
    # Only consider directories that actually contain files
    langs = []
    for d in os.listdir(answers_dir):
        d_path = os.path.join(answers_dir, d)
        if os.path.isdir(d_path) and any(os.path.isfile(os.path.join(d_path, f)) for f in os.listdir(d_path)):
            langs.append(d)
    
    if not langs:
        return None
        
    all_tests = []
    
    # Header
    makefile_content = ".PHONY: all test " + " ".join([f"test-{l}" for l in langs]) + "\n\n"
    makefile_content += "all: test\n\n"
    
    for lang in langs:
        lang_dir = os.path.join(answers_dir, lang)
        files = os.listdir(lang_dir)
        
        target_name = f"test-{lang}"
        
        cmd_compile = ""
        cmd_run = ""
        clean_files = []
        skip = False
        
        if lang == "c":
            if not is_tool("gcc"): skip = True
            sol_file = next((f for f in files if f.endswith(".c")), "solution.c")
            cmd_compile = f"gcc {os.path.join('answers/c', sol_file)} -O3 -lm -o solution_c"
            cmd_run = "./solution_c"
            clean_files.append("solution_c")
        elif lang == "cpp":
            if not is_tool("g++"): skip = True
            sol_file = next((f for f in files if f.endswith(".cpp")), "solution.cpp")
            cmd_compile = f"g++ {os.path.join('answers/cpp', sol_file)} -O3 -o solution_cpp"
            cmd_run = "./solution_cpp"
            clean_files.append("solution_cpp")
        elif lang == "java":
            if not is_tool("javac"): skip = True
            sol_file = next((f for f in files if f.endswith(".java")), "Solution.java")
            class_name = sol_file.replace(".java", "")
            cmd_compile = f"javac {os.path.join('answers/java', sol_file)}"
            cmd_run = f"java -cp answers/java {class_name}"
            clean_files.append(os.path.join("answers/java", "*.class"))
        elif lang == "py3":
            if not is_tool("python3"): skip = True
            sol_file = next((f for f in files if f.endswith(".py")), "solution.py")
            cmd_run = f"python3 {os.path.join('answers/py3', sol_file)}"
        elif lang == "kt":
            if not is_tool("kotlinc"): skip = True
            sol_file = next((f for f in files if f.endswith(".kt")), "Solution.kt")
            cmd_compile = f"kotlinc {os.path.join('answers/kt', sol_file)} -include-runtime -d solution_kt.jar"
            cmd_run = "java -jar solution_kt.jar"
            clean_files.append("solution_kt.jar")
        
        if skip or not cmd_run:
            continue
            
        all_tests.append(target_name)
        makefile_content += f"{target_name}:\n"
        if cmd_compile:
            makefile_content += f"\t{cmd_compile}\n"
        
        makefile_content += f"\t@echo \"Testing {problem_name} in {lang}...\"\n"
        makefile_content += f"\t@count=0; for f in $$(ls input/); do \\\n"
        makefile_content += f"\t\tif [ \$\$count -ge 3 ]; then break; fi; \\\n"
        makefile_content += f"\t\t{cmd_run} < input/\$\$f > tmp_out; \\\n"
        
        # Check if custom validator exists in compare/
        compare_script = os.path.join("compare", lang if lang != "py3" else "py3")
        if os.path.exists(os.path.join(problem_path, compare_script)):
            # Validator args: team_output, jury_output, problem_input
            # BOCA compare scripts return 4 for PASS, 6 for FAIL
            makefile_content += f"\t\tbash {compare_script} tmp_out output/\$\$f input/\$\$f > /dev/null 2> /dev/null; \\\n"
            makefile_content += f"\t\tif [ \$\$? -ne 4 ]; then echo \"Fail \$\$f in {lang}\"; fi; \\\n"
        else:
            makefile_content += f"\t\tdiff -q tmp_out output/\$\$f || echo \"Fail \$\$f in {lang}\"; \\\n"
            
        makefile_content += f"\t\tcount=\$\$((count+1)); \\\n"
        makefile_content += f"\tdone\n"
        if clean_files:
            makefile_content += f"\t@rm -f tmp_out {' '.join(clean_files)}\n"
        else:
            makefile_content += f"\t@rm -f tmp_out\n"
        makefile_content += "\n"

    makefile_content += "test: " + " ".join(all_tests) + "\n"
    
    makefile_path = os.path.join(problem_path, "Makefile")
    with open(makefile_path, "w") as f:
        f.write(makefile_content)
    
    return makefile_path

def main():
    base_dir = "/home/luis/Documents/puc/code-competitive/2025"
    problems = sorted([d for d in os.listdir(base_dir) if os.path.isdir(os.path.join(base_dir, d))])
    problems = [p for p in problems if p >= "F"]
    
    results = {}
    
    for problem in problems:
        print(f"--- Problem {problem} ---")
        problem_path = os.path.join(base_dir, problem)
        mk = create_makefile(problem_path, problem)
        
        results[problem] = {}
        
        if mk:
            # Get list of targets to run individually for better reporting
            with open(mk, 'r') as f:
                content = f.read()
            targets = [line.split(':')[0] for line in content.splitlines() if line.startswith('test-')]
            
            for target in targets:
                lang = target.replace('test-', '')
                print(f"Running {target}...")
                res = subprocess.run(["make", "-C", problem_path, target], capture_output=True, text=True)
                
                if "Fail" in res.stdout or res.returncode != 0:
                    results[problem][lang] = "FAIL"
                    print(f"Result {lang}: FAIL")
                    # Optionally find which test failed
                    fails = [line for line in res.stdout.splitlines() if "Fail" in line]
                    if fails:
                        print("  " + "\n  ".join(fails[:5])) # Show first 5 fails
                else:
                    results[problem][lang] = "PASS"
                    print(f"Result {lang}: PASS")
        else:
            print("No solutions found or compilers missing.")

    print("\n" + "="*40)
    print(f"{'Problem':<10} | {'Status per Language'}")
    print("-" * 40)
    for problem, langs in results.items():
        if not langs:
            status_str = "No solutions/compilers"
        else:
            status_str = ", ".join([f"{l}: {s}" for l, s in langs.items()])
        print(f"{problem:<10} | {status_str}")
    print("="*40)

if __name__ == "__main__":
    main()
