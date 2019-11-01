msg=$1
if [ -n "$msg" ]; then
   git add -A
   git commit -m"${msg}"
   git push
   git status
   echo "完成add、commit，push，文件已全部上传！"
else
    echo "请添加注释再来一遍"
fi
