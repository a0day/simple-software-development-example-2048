# Nums.gd - 纯数据类，不需要挂载到节点
class_name Nums

# 0. 定义枚举变量
enum Dir { 
	LEFT, 
	RIGHT, 
	UP, 
	DOWN 
}
# 1. 成员变量（相当于 C++ 的 private 成员）
var _grid: Array = []
var _score: int = 0

# 2. 构造函数（相当于 C++ 的 Constructor）
func _init():
	reset()

# 3. 成员方法（相当于 C++ 的 public 方法）
func get_cell(row: int, col: int) -> int:
	return _grid[row][col]

func get_score() -> int :
	return _score

func reset() -> void:
	_grid = [
		[0, 0, 0, 0],
		[0, 0, 0, 0],
		[0, 0, 0, 0],
		[0, 0, 0, 0]
	]
	spawn()
	spawn()
	_score = 0
	
# 4. 核心逻辑方法
func spawn() -> void:
	# 随机找一个 0 的位置，填入 2 或 4 仅在有效移动后使用
	var rx = randi_range(0,3)
	var ry = randi_range(0,3)
	while _grid[rx][ry] != 0:
		rx = randi_range(0,3)
		ry = randi_range(0,3)
	
	if randf() <= 0.1:
		_grid[rx][ry] = 4
	else:
		_grid[rx][ry] = 2
		
func move(direction: Dir) -> bool:
	match direction :
		Dir.LEFT:
			var flag = false
			for i in range(0,4):
				if _processline(_grid[i]):
					flag = true
			return flag
				
		Dir.RIGHT:
			var flag = false
			var t = [0,0,0,0]
			for i in range(0,4):
				for j in range(0,4):
					t[j] = _grid[i][3-j]
				if _processline(t):
					flag = true
				for j in range(0,4):
					_grid[i][3-j] = t[j]
			return flag
		Dir.UP:
			var flag = false
			var t = [0,0,0,0]
			for j in range(0,4):
				for i in range(0,4):
					t[i] = _grid[i][j]
				if _processline(t):
					flag = true
				for i in range(0,4):
					_grid[i][j] = t[i]
			return flag
		Dir.DOWN:
			var flag = false
			var t =[0,0,0,0]
			for j in range(0,4):
				for i in range(0,4):
					t[i] = _grid[3-i][j]
				if _processline(t):
					flag = true
				for i in range(0,4):
					_grid[3-i][j] = t[i]
			return flag
		_:
			return false

	
func _processline(t: Array) -> bool:
	# 对一个四元素数组 flag反映有效移动
	var flag: bool = false
	if _slide(t):
		flag = true
	if _merge(t):
		flag = true
	if _slide(t):
		flag = true 
	return flag
	
func _slide(t: Array) -> bool:
	#滑掉0
	var flag = false
	for i in range(1,4):
		if t[i] == 0:
			continue
		var tmp = i
		while tmp - 1 >= 0 && t[tmp - 1] == 0:
			tmp -= 1
		if tmp != i:
			t[tmp] = t[i]
			t[i] = 0
			flag = true	
	return flag

func _merge(t: Array) -> bool:
	#合并 仅在_slide后使用
	var flag = false
	var i = 0
	while i < 3 && t[i] != 0:
		if t[i] == t[i+1]:
			t[i] *= 2
			t[i+1] = 0
			_score += t[i]
			flag = true
			i += 2
		else:
			i += 1
	return flag
	
func isWin() -> bool:
	#search for 2048
	for i in range(0,4):
		for j in range(0,4):
			if _grid[i][j] == 2048:
				return true
	return false
	
	
func isLose() -> bool:
	#use after isWin
	for i in range(0,4):
		for j in range(0,4):
			if _grid[i][j] == 0:
				return false
			if i < 3 && _grid[i][j] == _grid[i+1][j]:
				return false
			if j < 3 && _grid[i][j] == _grid[i][j+1]:
				return false
	return true
