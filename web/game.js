class Nums {
    #value; 
    #score;

    constructor() {
        this.reset();
    }
    reset(){
        this.#value = [];
        for(let r=0; r<4;++r){
            this.#value.push(Array(4).fill(0));
        }
        this.#score =0;
        this.placeRandom();
        this.placeRandom();
    }

    getScore(){
        return this.#score;
    }
    get(row, col){
        return this.#value[row][col];
    }
    
    //use when valid move
    placeRandom() {
        let x,y;
        do{
            x = Math.floor(Math.random() * 4);
            y = Math.floor(Math.random() * 4);
        }while(this.#value[x][y] !== 0);
        this.#value[x][y] = Math.random() < 0.1 ? 4 : 2;
    }

    move(dir){
        let validmove = false;
        switch(dir){
            case "up":{
                for(let c=0;c<4;++c){
                    let tmpval = [];
                    for(let r=0;r<4;++r){
                        tmpval.push(this.#value[r][c]);
                    }
                    if(this.#processline(tmpval) === true){
                        validmove = true;
                        for(let r=0;r<4;++r){
                            this.#value[r][c] = tmpval[r];
                        }
                    }
                }
                return validmove;
            }
            case "down":{
                for(let c=0;c<4;++c){
                    let tmpval = [];
                    for(let r=0;r<4;++r){
                        tmpval.push(this.#value[3-r][c]);
                    }
                    if(this.#processline(tmpval) === true){
                        validmove = true;
                        for(let r=0;r<4;++r){
                            this.#value[3-r][c] = tmpval[r];
                        }
                    }
                }
                return validmove;
            }
            case "left":{
                for(let r=0;r<4;++r){
                    if(this.#processline(this.#value[r]) === true){
                        validmove = true;
                    }
                }
                return validmove;
            }
            case "right": {
                for(let r=0;r<4;++r){
                    let tmpval = [];
                    for(let c=0;c<4;++c){
                        tmpval.push(this.#value[r][3-c]);
                    }
                    if(this.#processline(tmpval) === true){
                        validmove = true;
                        for(let c=0;c<4;++c){
                            this.#value[r][3-c] = tmpval[c];
                        }
                    }
                }
                return validmove;
            }
            default: return false;
        }
    }
    #processline(line){
        let flag = false;
        if(this.#slide(line) === true) flag = true;
        if(this.#merge(line) === true) flag = true;
        if(this.#slide(line) === true) flag = true;
        return flag;
    }
    #slide(line){
        let flag = false;
        for(let r=1;r<4;++r){
            if(line[r] === 0){
                continue;
            }
            let tmp = r;
            while((tmp-1)>=0 && line[tmp-1] === 0){
                --tmp;
            }
            if(tmp !== r){
                line[tmp] = line[r];
                line[r] = 0;
                flag = true;
            }
        }
        return flag;
    }
    //use merge after slide
    #merge(line){
        let flag = false;
        let r=0;
        while(r<3 && line[r] !== 0){
            if(line[r] === line[r+1]){
                line[r] *= 2;
                this.#score += line[r];
                line[r+1] = 0;
                flag = true;
                r += 2;
            }
            else ++r;
        }
        return flag;
    }

    isLose(){
        for(let i=0; i<4; ++i){
            for(let j=0; j<4; ++j){
                if(this.#value[i][j] === 0)
                    return false;
                if(i < 3 && this.#value[i][j] === this.#value[i+1][j])
                    return false;
                if(j < 3 && this.#value[i][j] === this.#value[i][j+1])
                    return false;
            }
        }
        return true;
    }

    isWin(){
        for(let i = 0; i<4; ++i){
            for(let j=0; j<4; ++j){
                if(this.#value[i][j] === 2048)
                    return true;
            }
        }
        return false;
    }
}
