# simple software development example 2048

## 概述
本项目旨在通过开发2048游戏的过程，了解简单软件开发的流程。
项目中2048游戏开发的流程为：**终端版2048（c++） -> 图形化2048(c++ SDL3 SDL3_ttf) -> Web版2048**

## 游戏规则讲解
对于一个4x4空棋盘，初始时在两个位置生成数字，通过上下左右移动合并相等数字，每次有效移动才会再生成一次数字，直到最大值为2048或棋盘填满至无法通过移动合并。
生成数字：随机位置；90%为2，10%为4。
有效移动：一次移动指令中有数字位置变化或发生了合并。

## 终端版2048 (cli)
**编程语言及工具：** c++  cmake
**核心文件树（部分文件整合入cli/）**
```text
2048/
├── CMakeLists.txt
├── README.md
├── canvas.h      # 画布：棋盘框架与数字渲染
├── game.h        # Game 类：整合画布与数字
├── nums.h        # 棋盘数据、移动、胜负判断
├── nums.cc
├── main.cc       # 交互入口
└── build/        # CMake 构建产物
```
### 一些核心逻辑实现
**游戏层**：
使用random库进行随机位置与数字的生成（引擎 mt19937 产生原始的均匀随机数序列；  分布 uniform_int_distribution<int> 负责把引擎的原始输出映射成需要的范围； 构造临时random_device对象作为seed）
硬编码WASD作为移动指令，通过slide->merge->slide的形式完成一次移动合并，其中slide和merge的输入参数为int[4],返回bool以判定有效移动。

**交互层**
整个游戏由两个while循环嵌套形成，外层控制进入新游戏和退出程序，内层控制进入游戏后每次移动与退出当前游戏。
通过getline(cin,string)实现每次读取，严格判定输入长度与输入形式。

### 评价
交互较差 终端显示效果一般

## 图形化2048 (gui)
```
终端界面的2048棋盘完全由分隔符号组成，显示效果很差，同时键盘交互处理也很一般，由此我们引入SDL库初步实现图形化和优化键盘交互。
```
SDL 是一套具有广泛系统支持和语言支持的多媒体开发库，在本项目中用于窗口和图形管理（创建图形化窗口 渲染背景）和输入管理（处理键盘事件输入）。
SDL_ttf 作为SDL扩展库，在本项目中用于文本渲染。
**SDL3程序生命周期**
启动视频子系统 -> 创建window(窗口) -> 创建renderer(渲染器) -> 事件循环(每帧) -> 资源释放

**编程语言与工具**
c++ cmake 
SDL3 https://github.com/libsdl-org/SDL/releases
SDL3_ttf https://github.com/libsdl-org/SDL_ttf/releases

**核心文件树**
2048/
├── CMakeLists.txt          # 含 SDL3/SDL3_ttf 依赖与资源拷贝
├── README.md               
├── resources/
│   └── arial.ttf           # 图形版使用的字体文件
├── common/                 # 两个版本共享的核心游戏逻辑
│   ├── nums.h              
│   └── nums.cc                        
└── gui/                    # SDL3 图形版
    ├── main.cc             
    ├── board.h             
    └── board.cc 

### 显示层逻辑实现
项目中数字和游戏信息的渲染会经过 （int ->）string -> SDL_Surface (CPU 文字位图) -> SDL_Texture (GPU 纹理) ->renderer渲染
其中由于CPU上传GPU开销很大，故将用到的数字信息直接预渲染成常驻Texture内存，换掉每帧的重复计算。

SDL库的使用还包括算格子大小 字体大小 位置、分配颜色等操作。

### 评价
gui版的交互和显示相较cli有了大幅提升，但作为原生应用存在跨平台成本较大，发布更新成本较大，UI开发工具落后的问题。