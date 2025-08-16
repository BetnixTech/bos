
document.addEventListener("DOMContentLoaded", () => {
    const bootScreen = document.getElementById("boot-screen");
    const desktop = document.getElementById("desktop");
    const clock = document.getElementById("navbar-clock");

    setTimeout(() => { bootScreen.classList.add("hidden"); desktop.classList.remove("hidden"); }, 3000);
    function updateClock() { clock.textContent = new Date().toLocaleTimeString(); }
    setInterval(updateClock, 1000); updateClock();

    let clickTimeout;
    document.querySelectorAll(".icon").forEach(icon => {
        icon.addEventListener("click", () => {
            if (clickTimeout) { clearTimeout(clickTimeout); clickTimeout=null; openApp(icon.dataset.app); }
            else { clickTimeout = setTimeout(() => clickTimeout=null, 300); }
        });
    });

    function openApp(app) {
        const win = document.createElement("div"); win.classList.add("window");
        win.addEventListener("mousedown", e => {
            const offsetX=e.offsetX, offsetY=e.offsetY;
            function moveAt(e){win.style.left=e.pageX-offsetX+"px";win.style.top=e.pageY-offsetY+"px";}
            document.addEventListener("mousemove", moveAt);
            document.addEventListener("mouseup", ()=>document.removeEventListener("mousemove", moveAt),{once:true});
        });

        switch(app){
            case "calculator":
            win.innerHTML = `
                <div class="calc-grid">
                    <input type="text" id="calc-display" value="0" readonly>
                    <button class="calc-btn clear" data-value="clear">C</button>
                    <button class="calc-btn backspace" data-value="backspace">←</button>
                    <button class="calc-btn operator" data-value="/">/</button>
                    <button class="calc-btn number" data-value="7">7</button>
                    <button class="calc-btn number" data-value="8">8</button>
                    <button class="calc-btn number" data-value="9">9</button>
                    <button class="calc-btn operator" data-value="*">*</button>
                    <button class="calc-btn number" data-value="4">4</button>
                    <button class="calc-btn number" data-value="5">5</button>
                    <button class="calc-btn number" data-value="6">6</button>
                    <button class="calc-btn operator" data-value="-">-</button>
                    <button class="calc-btn number" data-value="1">1</button>
                    <button class="calc-btn number" data-value="2">2</button>
                    <button class="calc-btn number" data-value="3">3</button>
                    <button class="calc-btn operator" data-value="+">+</button>
                    <button class="calc-btn number" data-value="0">0</button>
                    <button class="calc-btn decimal" data-value=".">.</button>
                    <button class="calc-btn equals" data-value="=">=</button>
                </div>
            `;
        
            const display = win.querySelector("#calc-display");
            let currentInput = '0';
            let operator = null;
            let previousInput = null;
        
            win.querySelectorAll(".calc-btn").forEach(button => {
                button.addEventListener("click", () => {
                    const value = button.dataset.value;
        
                    if (button.classList.contains('number') || button.classList.contains('decimal')) {
                        if (currentInput === '0' && value !== '.') {
                            currentInput = value;
                        } else {
                            currentInput += value;
                        }
                        display.value = currentInput;
                    }
        
                    if (button.classList.contains('operator')) {
                        if (previousInput !== null) {
                            calculate();
                        }
                        operator = value;
                        previousInput = currentInput;
                        currentInput = '0';
                    }
        
                    if (value === '=') {
                        calculate();
                        operator = null;
                    }
        
                    if (value === 'clear') {
                        currentInput = '0';
                        operator = null;
                        previousInput = null;
                        display.value = currentInput;
                    }
        
                    if (value === 'backspace') {
                        currentInput = currentInput.slice(0, -1) || '0';
                        display.value = currentInput;
                    }
                });
            });
        
            function calculate() {
                const prev = parseFloat(previousInput);
                const current = parseFloat(currentInput);
                let result = 0;
        
                if (operator === '+') {
                    result = prev + current;
                } else if (operator === '-') {
                    result = prev - current;
                } else if (operator === '*') {
                    result = prev * current;
                } else if (operator === '/') {
                    if (current === 0) {
                        alert("Error: Division by zero");
                        return;
                    }
                    result = prev / current;
                }
        
                currentInput = String(result);
                display.value = currentInput;
                previousInput = null;
            }
            break;
            case "gallery":
                win.innerHTML = `<div id="gallery-container">
                  <img src="https://picsum.photos/100" style="margin:5px;">
                  <img src="https://picsum.photos/101" style="margin:5px;">
                  <img src="https://picsum.photos/102" style="margin:5px;">
                </div>`; break;
            case "files":
                win.innerHTML = `<div id="file-list"></div><input type="text" id="new-file" placeholder="New file name">
                <button id="create-file">Create File</button>`;
                const list=win.querySelector("#file-list");
                const inputFile=win.querySelector("#new-file");
                const button=win.querySelector("#create-file");
                function updateFiles(){list.innerHTML=""; const files=JSON.parse(localStorage.getItem("bnOS-files")||"[]"); files.forEach(f=>list.innerHTML+=`<div>${f}</div>`);}
                button.addEventListener("click",()=>{const files=JSON.parse(localStorage.getItem("bnOS-files")||"[]"); files.push(inputFile.value); localStorage.setItem("bnOS-files",JSON.stringify(files)); inputFile.value=""; updateFiles();});
                updateFiles(); break;
            case "terminal":
                win.innerHTML=`<div id="term-output"></div><input id="term-input" placeholder="type command" style="width:100%">`;
                const out=win.querySelector("#term-output");
                const termInput=win.querySelector("#term-input");
                termInput.addEventListener("keydown",e=>{if(e.key==="Enter"){const cmd=termInput.value; termInput.value="";
                    fetch("/run/python",{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify({code:cmd})})
                    .then(r=>r.json()).then(data=>{out.innerHTML+=`<div>${data.output||data.error}</div>`; out.scrollTop=out.scrollHeight;});}});
                break;
            case "settings":
                win.innerHTML=`<label>Brightness <input type="range" id="brightness2" min="0" max="100" value="100"></label>
                <label>Volume <input type="range" id="volume2" min="0" max="100" value="50"></label>`;
                win.querySelector("#brightness2").addEventListener("input",e=>document.body.style.filter=`brightness(${e.target.value}%)`);
                win.querySelector("#volume2").addEventListener("input",e=>console.log("Volume:",e.target.value)); break;
        }
        document.getElementById("windows-container").appendChild(win);
    }

    document.addEventListener("copy",e=>{e.clipboardData.setData("text/plain","bnOS clipboard text"); e.preventDefault();});
    document.addEventListener("paste",e=>{alert("Pasted: "+e.clipboardData.getData("text/plain")); e.preventDefault();});

    document.getElementById("brightness").addEventListener("input",e=>document.body.style.filter=`brightness(${e.target.value}%)`);
    document.getElementById("volume").addEventListener("input",e=>console.log("Volume:",e.target.value));
});
