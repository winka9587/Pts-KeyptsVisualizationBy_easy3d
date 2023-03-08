用于可视化5个点云
source pts+key
target pts+key
deform

bat脚本会搜索search_path下所有的ply文件, 当找到文件后对该目录运行exe，并将结果保存在save_path中。
有需要就修改bat文件中的路径

确保目录下包含这5个文件，会将最后一级目录的名字作为name，最后在save_path中保存1个图像
/xxx/xxx/xxx
xx/xxx/obj/
	preKeysinNOCS.ply
	viewPtsinNOCS.ply