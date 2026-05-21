import os
import re
import subprocess
import tempfile

def extract_code_blocks(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # regex to find code blocks
    cpp_pattern = re.compile(r'```cpp\n(.*?)```', re.DOTALL)
    py_pattern = re.compile(r'```python\n(.*?)```', re.DOTALL)
    
    cpp_blocks = cpp_pattern.findall(content)
    py_blocks = py_pattern.findall(content)
    
    return cpp_blocks, py_blocks

def verify_cpp(code):
    with tempfile.NamedTemporaryFile(suffix='.cpp', mode='w', delete=False) as f:
        f.write(code)
        temp_name = f.name
        
    try:
        # Check syntax using g++ -fsyntax-only
        res = subprocess.run(
            ['g++', '-std=c++17', '-fsyntax-only', temp_name],
            capture_output=True,
            text=True
        )
        if res.returncode != 0:
            return False, res.stderr
        return True, ""
    finally:
        if os.path.exists(temp_name):
            os.remove(temp_name)

def verify_python(code):
    with tempfile.NamedTemporaryFile(suffix='.py', mode='w', delete=False) as f:
        f.write(code)
        temp_name = f.name
        
    try:
        # Check syntax using python3 -m py_compile
        res = subprocess.run(
            ['python3', '-m', 'py_compile', temp_name],
            capture_output=True,
            text=True
        )
        if res.returncode != 0:
            return False, res.stderr
        return True, ""
    finally:
        if os.path.exists(temp_name):
            os.remove(temp_name)

def main():
    docs_dir = "/home/jao/code/code-competitive/docs"
    categories = ["data-structure", "algorithms", "math"]
    
    total_files = 0
    passed_files = 0
    failed_files = 0
    
    print("="*60)
    print("Code Syntax Verification Runner (Concatenated Mode)")
    print("="*60)
    
    for cat in categories:
        cat_dir = os.path.join(docs_dir, cat)
        if not os.path.exists(cat_dir):
            continue
            
        for file in sorted(os.listdir(cat_dir)):
            if not file.endswith('.md'):
                continue
                
            total_files += 1
            filepath = os.path.join(cat_dir, file)
            cpp_blocks, py_blocks = extract_code_blocks(filepath)
            
            file_ok = True
            errors = []
            
            # Verify C++ blocks concatenated
            if cpp_blocks:
                combined_cpp = "\n\n// --- NEXT BLOCK --- \n\n".join(cpp_blocks)
                ok, err = verify_cpp(combined_cpp)
                if not ok:
                    file_ok = False
                    errors.append(f"Combined C++ Compilation Failed:\n{err}")
            
            # Verify Python blocks concatenated
            if py_blocks:
                combined_py = "\n\n# --- NEXT BLOCK --- \n\n".join(py_blocks)
                ok, err = verify_python(combined_py)
                if not ok:
                    file_ok = False
                    errors.append(f"Combined Python Compilation Failed:\n{err}")
                    
            if file_ok:
                print(f"[PASS] docs/{cat}/{file}")
                passed_files += 1
            else:
                print(f"[FAIL] docs/{cat}/{file}")
                failed_files += 1
                for err in errors:
                    print("  " + "\n  ".join(err.splitlines()[:10])) # show first 10 lines of error
                    
    print("="*60)
    print(f"Summary: {total_files} files checked. {passed_files} passed, {failed_files} failed.")
    print("="*60)

if __name__ == '__main__':
    main()
