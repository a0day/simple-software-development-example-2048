# BGboard.gd —— 棋盘视图：铺静态底格
#
# 职责边界：只管"把棋盘画出来"。不碰游戏规则，不接输入。
# 坐标一律向 GridGeom 要 —— 这个脚本里不出现任何坐标算式。
# 公式只能存在于 GridGeom.gd 一处，否则哪天改了 Cell，两套坐标就打架。
#
# 挂载节点：board (Control)
# 它的矩形必须和底板 ColorRect 完全重合（都是从 (0,0) 起、410×410）。
# 因为 get_position() 返回的是"含最外层边距"的绝对坐标，
# Control 自己偏 10px，16 个格子就整体跟着偏 10px。

extends Control

# 2048 经典空格色 #cdc1b4
const CELL_COLOR_HEX := "cdc1b4"


func _ready() -> void:
	_build_grid()

# 铺 GRID_COUNT × GRID_COUNT 个底格。只在初始化时跑一次。
func _build_grid() -> void:
	var cell_size := Vector2(globalconfig.Cell, globalconfig.Cell)
	for row in range(globalconfig.GRID_COUNT):
		for col in range(globalconfig.GRID_COUNT):
			var cell := ColorRect.new()
			cell.color = Color(CELL_COLOR_HEX)
			# 位置和尺寸都来自唯一的那一处公式
			cell.position = globalconfig.get_position(row, col)
			cell.size = cell_size
			add_child(cell)
