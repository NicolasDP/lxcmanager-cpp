#! /bin/bash

repo_folder=repo

init_websocket()
{
    if [ ! -d $repo_folder/websocket ]; then
        git clone https://github.com/zaphoyd/websocketpp.git $repo_folder/websocket
    fi
}

init_boost()
{
    if [ ! -d $repo_folder/websocket ]; then
        svn co http://svn.boost.org/svn/boost/trunk $repo_folder/boost
    fi
}

update_websocket()
{
    init_websocket

    cd $repo_folder/websocket
    git pull
    cd ../..
}

update_boost()
{
    init_boost

    cd $repo_folder/boost
    svn up
    cd ../..
}

build_websocket()
{
    init_websocket

    make websocket
}

build_boost()
{
    init_boost

    make boost
}

delete_websocket()
{
    rm -rf $repo_folder/websocket
}

delete_boost()
{
    rm -rf $repo_folder/boost
}

case "$1" in
  init)
        case "$2" in
            websocket)
                init_websocket
                ;;
            boost)
                init_boost
                ;;
            *)
                init_websocket
                init_boost
                ;;
        esac
        echo "========================="
	echo "Repositories initialized"
        echo "========================="
	;;
  update)
        case "$2" in
            websocket)
                update_websocket
                ;;
            boost)
                update_boost
                ;;
            *)
                update_websocket
                update_boost
                ;;
        esac
        echo "========================="
	echo "Repositories updated"
        echo "========================="
	;;
  build)
        case "$2" in
            websocket)
                build_websocket
                ;;
            boost)
                build_boost
                ;;
            *)
                build_websocket
                build_boost
                ;;
        esac
        echo "========================="
        echo "Repositories builded"
        echo "========================="
        ;;
    delete)
        case "$2" in
            websocket)
                delete_websocket
                ;;
            boost)
                delete_boost
                ;;
            *)
                delete_websocket
                delete_boost
                ;;
        esac
        echo "========================="
        echo "Repositories deleted"
        echo "========================="
        ;;
  *)
	echo "Usage: launch.sh {init|update|build|delete} {boost|websocket}"
	exit 1
    ;;
esac
exit 0
