### 环境

vs2017内安装QT：

1.下载qt5.12.8版本安装包，安装时勾选MSVC 2017 x64

2.vs菜单栏选择扩展--->管理扩展，搜索Qt vs tools，安装

3.vs菜单栏扩展--->QT vs tools--->qt option ,add--->path ,path选择安装qt msvc2017_64的路径，version_name任意填即可。最后点击ok.

4.clone本项目到本地后，打开解决方案，选中该解决方案，右键选择 change_qt_version,

这样会使项目设置中的路径变成你本地的引用路径

### 分支

按照传统的流程，分为master,dev,feature三类分支，

若已经确定好某个功能，从dev创建的feature_xx分支进行开发，本地测试成功后， 将本地(源分支)feature_xx Push到远端，然后在GitLab指定目标分支（dev branch）发起Merge Request，我会进行merge操作。

若没有确定好功能，可以直接从dev提交合并请求。

大家最好在gitlab上对项目进行通知设置，若master或dev分支发生变化，请及时拉取到本地

### Project_ICEM_Graphics文件目录

- Loader:解析不同模型文件

- Model:模型容器类
- Widgets:控件类
- Res:资源文件

ps:编码规范文档以及参考资料放在pc_common_doc内，希望大家能够将自己学习过程中遇到的好资料或者自己的总结都能放进去。











