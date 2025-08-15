
from flask import Flask, request, jsonify
import subprocess, tempfile, os

app = Flask(__name__)

@app.route("/run/python", methods=["POST"])
def run_python():
    code = request.json.get("code","")
    try:
        exec_globals = {}
        exec(code, exec_globals)
        return jsonify({"output": str(exec_globals)})
    except Exception as e:
        return jsonify({"error": str(e)})

@app.route("/run/cpp", methods=["POST"])
def run_cpp():
    code = request.json.get("code","")
    with tempfile.NamedTemporaryFile(delete=False,suffix=".cpp") as f:
        f.write(code.encode()); f.flush()
        exe_file=f.name.replace(".cpp","")
        try:
            subprocess.run(["g++", f.name, "-o", exe_file], check=True)
            result=subprocess.run([exe_file], capture_output=True, text=True)
            return jsonify({"output": result.stdout})
        except Exception as e:
            return jsonify({"error": str(e)})
        finally:
            os.remove(f.name)
            if os.path.exists(exe_file): os.remove(exe_file)

@app.route("/run/rust", methods=["POST"])
def run_rust():
    code = request.json.get("code","")
    with tempfile.TemporaryDirectory() as tmpdir:
        src = os.path.join(tmpdir, "main.rs")
        with open(src,"w") as f: f.write(code)
        exe_file = os.path.join(tmpdir, "main")
        try:
            subprocess.run(["rustc", src, "-o", exe_file], check=True)
            result=subprocess.run([exe_file], capture_output=True, text=True)
            return jsonify({"output": result.stdout})
        except Exception as e:
            return jsonify({"error": str(e)})
