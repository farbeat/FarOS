本文档为生成emmc文件镜像的使用指导

1.如果想要生成文件镜像，需要在对应target中添加配置:'fs_image': True
2.当前目录适配的是emmc的FAT文件镜像。
3.生成emmc的FAT文件镜像前，请打开mkfatimgtool.py脚本进行相关配置，详细见脚本说明
4.将要烧录的资源文件放到fs目录中，可以有子目录，此目录对应A核系统挂载第一分区的目录，默认为system目录。
5.因为生成FAT镜像的工具是windows平台的工具，生成过程需要在windows平台上进行。
6.执行脚本是python脚本，所以需要预先安装python3软件，建议python3.7及以上的版本。
7.执行python mkfatimgtool.py即可在当前目录生成FAT镜像，即file.bin文件。