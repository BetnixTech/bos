from flask import Flask, request, jsonify, send_file
import subprocess
import psutil
import os
import shutil

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
        # Split the command into a list of arguments for safety
        result = subprocess.run(command.split(), capture_output=True, text=True, check=True)
        return jsonify({"output": result.stdout})
    except FileNotFoundError:
        return jsonify({"error": f"Command not found: '{command.split()[0]}'"})
    except subprocess.CalledProcessError as e:
        return jsonify({"error": f"Command failed: {e.stderr}"})
    except Exception as e:
        return jsonify({"error": str(e)})

def get_storage_devices():
    devices = []
    for part in psutil.disk_partitions():
        # Skip system partitions
        if "rw" in part.opts and ("/media" in part.mountpoint or "/mnt" in part.mountpoint):
            devices.append({
                "device": part.device,
                "mountpoint": part.mountpoint,
                "fstype": part.fstype
            })
    return devices

# List files in a drive
def list_files(path):
    try:
        return os.listdir(path)
    except Exception as e:
        return {"error": str(e)}

@app.route("/drives", methods=["GET"])
def drives():
    return jsonify(get_storage_devices())

@app.route("/files/<path:filepath>", methods=["GET"])
def files(filepath):
    # Ensure absolute path
    full_path = os.path.join("/", filepath)

    if os.path.isfile(full_path):
        return send_file(full_path, as_attachment=True)
    elif os.path.isdir(full_path):
        return jsonify(os.listdir(full_path))
    else:
        return jsonify({"error": "Path not found"}), 404

# New route to create a new empty file
@app.route("/file", methods=["POST"])
def create_file():
    path = request.json.get("path", "")
    full_path = os.path.join(os.getcwd(), path.strip('/'))
    try:
        open(full_path, 'a').close()
        return jsonify({"status": "success"})
    except Exception as e:
        return jsonify({"error": str(e)})

# New route to create a new folder
@app.route("/folder", methods=["POST"])
def create_folder():
    path = request.json.get("path", "")
    full_path = os.path.join(os.getcwd(), path.strip('/'))
    try:
        os.makedirs(full_path)
        return jsonify({"status": "success"})
    except Exception as e:
        return jsonify({"error": str(e)})

# New route to upload a file
@app.route("/upload", methods=["POST"])
def upload_file():
    if 'file' not in request.files:
        return jsonify({"error": "No file part"}), 400
    file = request.files['file']
    path = request.form.get("path", "")
    full_path = os.path.join(os.getcwd(), path.strip('/'), file.filename)
    try:
        file.save(full_path)
        return jsonify({"status": "success"})
    except Exception as e:
        return jsonify({"error": str(e)})

# New route to delete a file or folder
@app.route("/delete", methods=["POST"])
def delete_item():
    path = request.json.get("path", "")
    full_path = os.path.join(os.getcwd(), path.strip('/'))
    try:
        if os.path.isdir(full_path):
            shutil.rmtree(full_path)
        else:
            os.remove(full_path)
        return jsonify({"status": "success"})
    except Exception as e:
        return jsonify({"error": str(e)})

# Updated files route to list files and folders with metadata
@app.route("/files", methods=["GET"])
def files():
    path = request.args.get("path", "/")
    full_path = os.path.join(os.getcwd(), path.strip('/'))

    if not os.path.exists(full_path) or not os.path.isdir(full_path):
        return jsonify({"error": "Path not found or is not a directory"}), 404

    items = []
    for item in os.listdir(full_path):
        item_path = os.path.join(full_path, item)
        items.append({
            "name": item,
            "path": os.path.join(path, item),
            "is_dir": os.path.isdir(item_path)
        })
    return jsonify(items)



if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
