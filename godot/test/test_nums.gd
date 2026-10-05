# Nums.gd 的独立测试脚本
#
# ════════════════════════════════════════════════════════════════
# 怎么运行（在 PowerShell 里，路径换成你自己的）：
#
#   & "C:\Godot\Godot_v4.7-stable_win64.exe" --headless --path "C:\Users\34240\Desktop\2048\godot" --script res://test/test_nums.gd
#
#   --headless   不弹窗口
#   --path       工程根目录（省掉 cd）
#   --script     要执行的脚本（相对工程，必须用 res:// 开头）
#
# 退出码：0 = 全部通过，1 = 有失败用例（以后可以拿它接 CI）
# ════════════════════════════════════════════════════════════════
#
# 三条设计说明：
#
# 1. 不依赖主循环、不依赖输入层 —— 直接造局面调 move()。
#    这就是"逻辑与表现分离"换来测试能力的兑现。
#
# 2. 每个用例前都整体替换 _grid：Nums.new() 会走 _init() -> reset()，
#    随机铺两个方块；不覆盖掉的话，你的期望棋盘永远对不上，而且失败
#    还会随机出现（这种"偶发失败"最难查）。
#
# 3. 棋盘比较用 str() 转成字符串再比：
#    不依赖 Array 的 == 到底是什么语义（GDScript 的引用/值语义很容易记错），
#    而且失败时能把"期望"和"实际"两行直接打出来 —— 测试的可读性比优雅重要。

extends SceneTree

var _passed := 0
var _failed := 0


func _init() -> void:
	print("========== Nums.gd 测试 ==========")
	_test_move()
	_test_win_lose()
	_test_spawn()
	_report()
	quit(1 if _failed > 0 else 0)


# ---------------------------------------------------------------- 工具

func _make_nums(grid: Array) -> Nums:
	# 注意 duplicate(true)：GDScript 的 Array 是引用语义，
	# 不深拷贝的话，move() 会把上面 cases 里写死的字面量给改掉，
	# 后面的用例就会拿到被污染的输入。
	var n := Nums.new()
	n._grid = grid.duplicate(true)
	n._score = 0
	return n


func _check(label: String, actual, expected) -> void:
	if str(actual) == str(expected):
		_passed += 1
		print("  [PASS] " + label)
	else:
		_failed += 1
		print("  [FAIL] " + label)
		print("           期望: " + str(expected))
		print("           实际: " + str(actual))


func _report() -> void:
	print("")
	print("==================================")
	print("通过 " + str(_passed) + " 项，失败 " + str(_failed) + " 项")
	if _failed > 0:
		print("上面标 [FAIL] 的就是没过的，先修逻辑，别改期望值")
	print("==================================")


# ---------------------------------------------------------------- move()

func _test_move() -> void:
	print("")
	print("--- move() ---")

	var cases := [
		{
			"label": "LEFT  [2,2,2,2] -> [4,4,0,0] （一回合只合并一次）",
			"grid": [[2, 2, 2, 2], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"dir": Nums.Dir.LEFT,
			"expect": [[4, 4, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"moved": true,
			"score": 8,
		},
		{
			"label": "LEFT  [2,2,4,0] -> [4,4,0,0] （不跨块合并）",
			"grid": [[2, 2, 4, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"dir": Nums.Dir.LEFT,
			"expect": [[4, 4, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"moved": true,
			"score": 4,
		},
		{
			"label": "LEFT  [4,4,4,4] -> [8,8,0,0]",
			"grid": [[4, 4, 4, 4], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"dir": Nums.Dir.LEFT,
			"expect": [[8, 8, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"moved": true,
			"score": 16,
		},
		{
			"label": "RIGHT [2,0,0,2] -> [0,0,0,4] （镜像 + 写回）",
			"grid": [[2, 0, 0, 2], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"dir": Nums.Dir.RIGHT,
			"expect": [[0, 0, 0, 4], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"moved": true,
			"score": 4,
		},
		{
			"label": "RIGHT [2,4,0,0] -> [0,0,2,4] （只平移不合并，专测写回方向）",
			"grid": [[2, 4, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"dir": Nums.Dir.RIGHT,
			"expect": [[0, 0, 2, 4], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"moved": true,
			"score": 0,
		},
		{
			"label": "UP    第0列 [2,2,0,0] -> 第0列 [4,0,0,0]",
			"grid": [[2, 0, 0, 0], [2, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"dir": Nums.Dir.UP,
			"expect": [[4, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"moved": true,
			"score": 4,
		},
		{
			"label": "DOWN  第0列 [2,2,0,0] -> 第0列 [0,0,0,4]",
			"grid": [[2, 0, 0, 0], [2, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"dir": Nums.Dir.DOWN,
			"expect": [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [4, 0, 0, 0]],
			"moved": true,
			"score": 4,
		},
		{
			"label": "LEFT  [2,4,0,0] 已在最左 -> 无位移，返回 false",
			"grid": [[2, 4, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"dir": Nums.Dir.LEFT,
			"expect": [[2, 4, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"moved": false,
			"score": 0,
		},
		{
			"label": "LEFT  全 0 棋盘 -> 无位移，返回 false",
			"grid": [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"dir": Nums.Dir.LEFT,
			"expect": [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]],
			"moved": false,
			"score": 0,
		},
	]

	for c in cases:
		var n := _make_nums(c.grid)
		var moved: bool = n.move(c.dir)
		_check(c.label + "  ·  棋盘", n._grid, c.expect)
		_check(c.label + "  ·  返回值", moved, c.moved)
		_check(c.label + "  ·  分数", n.get_score(), c.score)


# ---------------------------------------------------------------- isWin() / isLose()

func _test_win_lose() -> void:
	print("")
	print("--- isWin() / isLose() ---")

	var win_grid := [
		[2048, 0, 0, 0],
		[0, 0, 0, 0],
		[0, 0, 0, 0],
		[0, 0, 0, 0],
	]
	var n1 := _make_nums(win_grid)
	_check("出现 2048 -> isWin() 为 true", n1.isWin(), true)

	# 满盘，且任意相邻两格都不相等 -> 真的死局
	var dead_grid := [
		[2, 4, 2, 4],
		[4, 2, 4, 2],
		[2, 4, 2, 4],
		[4, 2, 4, 2],
	]
	var n2 := _make_nums(dead_grid)
	_check("满盘且无可合并 -> isLose() 为 true", n2.isLose(), true)
	_check("满盘且无可合并 -> isWin() 为 false", n2.isWin(), false)

	# 死局下四个方向都必须动不了 —— isLose 为 true 和 move 全 false 应该是同一件事
	for d in [Nums.Dir.LEFT, Nums.Dir.RIGHT, Nums.Dir.UP, Nums.Dir.DOWN]:
		var n2b := _make_nums(dead_grid)
		_check("死局下 move 返回 false（dir=" + str(d) + "）", n2b.move(d), false)

	# 满盘，但有一对相邻相等 -> 还能合，不算输
	var almost_grid := [
		[2, 2, 4, 8],
		[4, 8, 16, 32],
		[8, 16, 32, 64],
		[16, 32, 64, 128],
	]
	var n3 := _make_nums(almost_grid)
	_check("满盘但可合并 -> isLose() 为 false", n3.isLose(), false)
	_check("满盘但可合并 -> move(LEFT) 为 true", n3.move(Nums.Dir.LEFT), true)

	var n4 := _make_nums([[2, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]])
	_check("还有空位 -> isLose() 为 false", n4.isLose(), false)


# ---------------------------------------------------------------- spawn()

func _test_spawn() -> void:
	print("")
	print("--- spawn() ---")

	# 15 格填满，只剩 (3,3) 是空的 —— 于是"填哪一格"是确定的，
	# 只有"填 2 还是 4"是随机的。这样才测得出东西。
	var grid := [
		[2, 4, 2, 4],
		[4, 2, 4, 2],
		[2, 4, 2, 4],
		[4, 2, 4, 0],
	]
	var n := _make_nums(grid)
	n.spawn()
	var v: int = n.get_cell(3, 3)
	_check("唯一空位被填上 2 或 4", v == 2 or v == 4, true)
	var expect := grid.duplicate(true)
	expect[3][3] = v
	_check("除该格外其余 15 格不变", n._grid, expect)

	# 空棋盘上连调两次 -> 恰好两个非 0 格，且值只能是 2 或 4
	var n2 := _make_nums([[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]])
	n2.spawn()
	n2.spawn()
	var filled := 0
	var bad := 0
	for i in range(4):
		for j in range(4):
			var c: int = n2.get_cell(i, j)
			if c != 0:
				filled += 1
				if c != 2 and c != 4:
					bad += 1
	_check("连续 spawn 两次 -> 恰好 2 个非 0 格", filled, 2)
	_check("生成的值只能是 2 或 4", bad, 0)

	# ⚠ 已知问题，故意没有写用例：
	#   棋盘 16 格全满时，spawn() 里的 while _grid[rx][ry] != 0 永不退出 —— 死循环。
	#   这个用例现在**写不出来**，写下去测试进程就直接卡死。
	#   "因为会卡死而不敢写的测试"本身就是设计有问题的信号：
	#   给 spawn() 加一个"先收集空格、没空格就直接返回"的守卫，这里就能补上断言了。
