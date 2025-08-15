from flask import Flask, request, jsonify
import subprocess
import os

app = Flask(__name__)

# -----------------------------
# Run Python code
# -----------------------------
@app.route("/run/python", methods=["POST"])
def run_python():
    code = request.json.get("code", "")
    try:
        exec_globals = {}
        exec(code, exec_globals)
        return jsonify({"output": str(exec_globals)})
    except Exception as e:
        return jsonify({"error": str(e)})

# -----------------------------
# Run C code
# -----------------------------
@app.route("/run/c", methods=["POST"])
def run_c():
    code = request.json.get("code", "")
    c_file = "/mnt/data/bnOS/apps/temp.c"
    exe_file = "/mnt/data/bnOS/apps/temp"
    with open(c_file, "w") as f:
        f.write(code)
    try:
        subprocess.run(["gcc", c_file, "-o", exe_file], check=True)
        result = subprocess.run([exe_file], capture_output=True, text=True)
        return jsonify({"output": result.stdout})
    except Exception as e:
        return jsonify({"error": str(e)})
    finally:
        for f in [c_file, exe_file]:
            if os.path.exists(f):
                os.remove(f)

# -----------------------------
# Run C++ code
# -----------------------------
@app.route("/run/cpp", methods=["POST"])
def run_cpp():
    cpp_file = "/mnt/data/bnOS/apps/temp.cpp"
    exe_file = "/mnt/data/bnOS/apps/temp"
    code = request.json.get("code", "")
    with open(cpp_file, "w") as f:
        f.write(code)
    try:
        subprocess.run(["g++", cpp_file, "-o", exe_file], check=True)
        result = subprocess.run([exe_file], capture_output=True, text=True)
        return jsonify({"output": result.stdout})
    except Exception as e:
        return jsonify({"error": str(e)})
    finally:
        for f in [cpp_file, exe_file]:
            if os.path.exists(f):
                os.remove(f)

# -----------------------------
# Run Rust code
# -----------------------------
@app.route("/run/rust", methods=["POST"])
def run_rust():
    rust_file = "/mnt/data/bnOS/apps/temp.rs"
    exe_file = "/mnt/data/bnOS/apps/temp_rust"
    code = request.json.get("code", "")
    with open(rust_file, "w") as f:
        f.write(code)
    try:
        subprocess.run(["rustc", rust_file, "-o", exe_file], check=True)
        result = subprocess.run([exe_file], capture_output=True, text=True)
        return jsonify({"output": result.stdout})
    except Exception as e:
        return jsonify({"error": str(e)})
    finally:
        for f in [rust_file, exe_file]:
            if os.path.exists(f):
                os.remove(f)

# -----------------------------
# Run Java code
# -----------------------------
@app.route("/run/java", methods=["POST"])
def run_java():
    code = request.json.get("code", "")
    java_file = "/mnt/data/bnOS/apps/memory.java"
    class_file = "/mnt/data/bnOS/apps/memory.class"
    with open(java_file, "w") as f:
        f.write(code)
    try:
        subprocess.run(["javac", java_file], check=True)
        result = subprocess.run(["java", "-cp", "/mnt/data/bnOS/apps", "memory"], capture_output=True, text=True)
        return jsonify({"output": result.stdout})
    except Exception as e:
        return jsonify({"error": str(e)})
    finally:
        for f in [java_file, class_file]:
            if os.path.exists(f):
                os.remove(f)

# -----------------------------
# Run Assembly code (NASM x86-64)
# -----------------------------
@app.route("/run/asm", methods=["POST"])
def run_asm():
    code = request.json.get("code", "")
    asm_file = "/mnt/data/bnOS/apps/temp.asm"
    obj_file = "/mnt/data/bnOS/apps/temp.o"
    exe_file = "/mnt/data/bnOS/apps/temp_asm"
    with open(asm_file, "w") as f:
        f.write(code)
    try:
        subprocess.run(["nasm", "-felf64", asm_file, "-o", obj_file], check=True)
        subprocess.run(["ld", obj_file, "-o", exe_file], check=True)
        result = subprocess.run([exe_file], capture_output=True, text=True)
        return jsonify({"output": result.stdout})
    except Exception as e:
        return jsonify({"error": str(e)})
    finally:
        for f in [asm_file, obj_file, exe_file]:
            if os.path.exists(f):
                os.remove(f)

   @app.route("/toggle/wifi", methods=["POST"])
def toggle_wifi():
    state = request.json.get("state")
    try:
        # Linux example: use nmcli to control Wi-Fi
        if state == "on":
            subprocess.run(["nmcli", "radio", "wifi", "on"], check=True)
        else:
            subprocess.run(["nmcli", "radio", "wifi", "off"], check=True)
        return jsonify({"status": state})
    except Exception as e:
        return jsonify({"status": "error", "error": str(e)})

@app.route("/toggle/bluetooth", methods=["POST"])
def toggle_bluetooth():
    state = request.json.get("state")
    try:
        # Linux example: use rfkill to control Bluetooth
        if state == "on":
            subprocess.run(["rfkill", "unblock", "bluetooth"], check=True)
        else:
            subprocess.run(["rfkill", "block", "bluetooth"], check=True)
        return jsonify({"status": state})
    except Exception as e:
        return jsonify({"status": "error", "error": str(e)})
        
@app.route("/run/linux", methods=["POST"])
def run_linux():
    command = request.json.get("command", "")
    try:
        # Run the command on the host Linux system
        result = subprocess.run(command, shell=True, capture_output=True, text=True)
        return jsonify({"output": result.stdout + result.stderr})
    except Exception as e:
        return jsonify({"error": str(e)})


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
