// ============================================================
// class_demo.js —— JS 类与构造语法演示 + 迁移要点预览
// 运行方式：双击同目录的 class_demo.html
//（JS 文件不能像 exe 一样独立运行，需要网页这个"宿主"来加载它）
// ============================================================

// ---------- 一、引用 vs 拷贝（迁移坑之最，先看现场） ----------
// 结论：数组赋值传的是引用，不是副本。需要真拷贝时用 slice()。
function demoReference() {
  const a = [2, 0, 2, 4];
  const b = a;              // b 和 a 指向同一个数组！
  b[0] = 99;
  const c = a.slice();      // slice() 才是真拷贝
  c[1] = 77;
  return [
    "const b = a; b[0]=99;  之后 a = [" + a.join(", ") + "]   ← a 被连带改了",
    "const c = a.slice(); c[1]=77; 之后 a = [" + a.join(", ") + "]   ← a 安然无恙",
  ].join("\n");
}

// ---------- 二、二维数组：fill 的经典陷阱 ----------
function demo2D() {
  // 错误写法：Array(4).fill(同一个数组) —— 四行共享同一个数组
  const bad = Array(4).fill(Array(4).fill(0));
  bad[0][0] = 9;            // 明明只想改第一行……

  // 正确写法：每一行单独创建
  const good = [];
  for (let r = 0; r < 4; r++) {
    good.push(Array(4).fill(0));
  }
  good[0][0] = 9;

  return [
    "错误写法：bad[0][0]=9 后，bad[1][0] = " + bad[1][0] + "   ← 无辜遭殃",
    "正确写法：good[0][0]=9 后，good[1][0] = " + good[1][0] + "   ← 正常",
  ].join("\n");
}

// ---------- 三、类与构造：MiniBoard（迷你演示类） ----------
// 与你 common/nums.h 的对应关系：
//   #value / #score   ↔  private: int value[4][4]{0}; int score;
//   constructor()     ↔  Nums::Nums()
//   #placeRandom()    ↔  私有的 spawn()（此处仅演示语法，正式迁移你自己写）
//   get score         ↔  int getScore() const
//   get(row, col)     ↔  int get(int row, int col) const
class MiniBoard {
  #value;   // 私有字段：类外无法访问，等价 C++ 的 private:
  #score;

  constructor() {
    // 二维数组：逐行创建（原因见上面第二节）
    this.#value = [];
    for (let r = 0; r < 4; r++) {
      this.#value.push(Array(4).fill(0));
    }
    this.#score = 0;
    this.#placeRandom();   // 构造函数里调用私有方法，同你的 C++ 构造
    this.#placeRandom();
  }

  // 私有方法：# 开头，类外不可调用
  // Math.random() 返回 [0,1) 的浮点，映射你的随机源：
  //   Math.floor(Math.random() * 4)  ↔  xydist(rng)         0~3 的整数
  //   Math.random() < 0.1            ↔  numdist(rng) == 9   1/10 概率
  #placeRandom() {
    let x, y;
    do {                                // do-while 与 C++ 相同
      x = Math.floor(Math.random() * 4);
      y = Math.floor(Math.random() * 4);
    } while (this.#value[x][y] !== 0);  // 一律用 === / !== 严格比较
    this.#value[x][y] = Math.random() < 0.1 ? 4 : 2;  // 三目运算符也相同
  }

  // getter：外部用 board.score 读取（不带括号），对应 C++ getScore()
  get score() {
    return this.#score;
  }

  // 公有方法，对应 C++ 公有成员函数
  get(row, col) {
    return this.#value[row][col];
  }

  // 演示"把行传出去改，棋盘真的会变"——引用语义正是你
  // slide(int* t) 靠指针实现的效果，JS 里白送
  fillRow(row, n) {
    const line = this.#value[row];      // line 就是棋盘第 row 行本身
    for (let i = 0; i < 4; i++) {
      line[i] = n;                      // 改 line 就是改棋盘
    }
    return line === this.#value[row];   // true：证明是同一个数组
  }

  // 生成可读文本（对应你 CLI 版打印棋盘的函数）
  print() {
    const rows = [];
    for (let r = 0; r < 4; r++) {
      rows.push(this.#value[r].join("  "));
    }
    return rows.join("\n");
  }
}

// ---------- 四、动态类型速览 ----------
function demoTypes() {
  return [
    'typeof 2         → "' + typeof 2 + '"     （数字不分 int/double）',
    'typeof "2"       → "' + typeof "2" + '"',
    '2 === "2"        → ' + (2 === "2") + "                （严格比较）",
    '"2" == 2         → ' + ("2" == 2) + "                 （== 偷偷转型，禁用）",
    '"分数：" + 1 + 1  → "' + ("分数：" + 1 + 1) + '"      （+ 遇字符串变拼接）',
    'typeof undefined → "' + typeof undefined + '"',
  ].join("\n");
}

// ---------- 五、使用：对应 main 里的 Nums game; ----------
const board = new MiniBoard();          // new 和 C++ 一样，但不需要 delete
const refDemo = board.fillRow(0, 8);    // 把第 0 行填成 8（顺带演示引用语义）

const report = [
  "【一】引用 vs 拷贝", demoReference(), "",
  "【二】二维数组：fill 的坑", demo2D(), "",
  "【三】MiniBoard 演示类",
  "fillRow(0, 8) 返回 " + refDemo + "（true = 传出去的就是行本身，引用语义）",
  "score（getter，读取时不带括号）= " + board.score,
  "get(1, 1) = " + board.get(1, 1),
  "棋盘内容（第 0 行刚被 fillRow 改成 8）：", board.print(), "",
  "【四】动态类型速览", demoTypes(), "",
  "※ 每次刷新页面，随机数字的位置都会变——这就是 Math.random() 不可复现，",
  "  对应 mt19937 被换掉后无法固定种子，测试策略用之前讲的『金标准对照』。",
].join("\n");

// 输出到网页 <pre> 和控制台各一份，方便对照
document.getElementById("out").textContent = report;
console.log(report);
