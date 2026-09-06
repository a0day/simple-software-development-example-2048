const canvas = document.getElementById("board");
const score = document.getElementById("score");
const ctx = canvas.getContext('2d');

const WIDTH = canvas.width;
const HEIGHT = canvas.height;
const gap = 20;
const cell = (WIDTH - 5*gap)/4;

const bgColor = [250, 248, 239];
const cellColor = [205, 193, 180];
const bg =[
    [238, 228, 218], 
    [237, 224, 200], 
    [242, 177, 121], 
    [245, 149, 99], 
    [246, 124, 95], 
    [246, 94, 59], 
    [237, 207, 114], 
    [237, 204, 97], 
    [237, 200, 80], 
    [237, 197, 63], 
    [237, 194, 46]
];
const fg = [
  [119, 110, 101], 
  [119, 110, 101], 
  [249, 246, 242], 
  [249, 246, 242], 
  [249, 246, 242], 
  [249, 246, 242], 
  [249, 246, 242], 
  [249, 246, 242], 
  [249, 246, 242], 
  [249, 246, 242], 
  [249, 246, 242]
];

const nums = new Nums();

function draw(){
    ctx.fillStyle =rgb(bgColor);
    ctx.fillRect(0,0,WIDTH,HEIGHT);
   
    ctx.font = "30px sans-serif";
    ctx.textAlign="center";ctx.textBaseline="middle";

    for(let r=0;r<4;++r){
        for(let c=0;c<4;++c){
            drawCell(r, c, nums.get(r,c));
        }
    }
    score.textContent = nums.getScore();
}

function drawCell(r, c, val){
    let {x,y} = cellRect(r,c);
    
    ctx.fillStyle = rgb( val===0 ? cellColor : bg[Math.log2(val)-1]);
    ctx.beginPath();
    ctx.roundRect(x,y,cell,cell,6);
    ctx.fill();

    if (val !== 0){
        ctx.fillStyle =rgb(fg[Math.log2(val)-1]);
        ctx.fillText(val,x+cell/2,y+cell/2);
    }
}

function cellRect(row,col){
    return {x: col*cell+(col+1)*gap,y: row*cell+(row+1)*gap};
}

function rgb(color){
    return `rgb(${color[0]}, ${color[1]}, ${color[2]})`;
}


draw();
const keyMap={
    "w": "up", "arrowup": "up",
    "a": "left", "arrowleft": "left",
    "s": "down", "arrowdown": "down",
    "d": "right", "arrowright": "right" 
};
let state = "playing";

function restart(){
    nums.reset();
    state = "playing";
    overlay.classList.add("hidden");
    draw();
}

let overlay = document.getElementById("overlay");
let restex = document.getElementById("result-text");
let resbtn = document.getElementById("restart-btn");

resbtn.addEventListener("click", restart);
window.addEventListener("keydown",function(e){
    if(!e.repeat){
        const key = e.key.toLowerCase();
        if(key === "r"){
            restart();
        }
    
        else if(state === "playing") {
            const dir = keyMap[key];
            if(!dir) return;
            e.preventDefault();

            if(nums.move(dir) === true){
                nums.placeRandom();
            
                if(nums.isWin() === true){
                    state = "won";
                    restex.textContent = "you win!";
                    overlay.classList.remove("hidden");
                }
                else if(nums.isLose() === true){
                    state = "over";
                    restex.textContent = "game over";
                    overlay.classList.remove("hidden");
                }
            }
     
            draw();
        }
    }
});