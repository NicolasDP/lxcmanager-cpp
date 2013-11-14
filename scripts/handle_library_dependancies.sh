#! /bin/bash

COMMAND=${1}
COMMAND_ARGUMENT=${2}

if [ -f "./bash_basic_functions.sh" ]; then
    source ./bash_basic_functions.sh
elif [ -f "./scripts/bash_basic_functions.sh" ]; then
    source ./scripts/bash_basic_functions.sh
else
    echo "ERROR: Unable to find basic bash functions" >&2
    exit 1
fi

REPOS_FOLDER=repo
REPOS_LIST="boost websocket"

SCRIPT_NAME=`basename ${0}`

EXIT_VALUE=1

clone_repository()
{
    REPO_NAME=${1}
    RETURN_VALUE=0

    case "${REPO_NAME}" in
        websocket)
            if [ ! -d $REPOS_FOLDER/websocket ]; then
                git clone https://github.com/zaphoyd/websocketpp.git \
                          ${REPOS_FOLDER}/websocket
                RETURN_VALUE=${?}
            fi
            ;;
        boost)
            if [ ! -d $REPOS_FOLDER/boost ]; then
                svn co http://svn.boost.org/svn/boost/trunk $REPOS_FOLDER/boost
                RETURN_VALUE=${?}
            fi
            ;;
        *)
            RETURN_VALUE=1
            ;;
    esac

    if [ ${RETURN_VALUE} -eq 0 ]; then
        message_info "${REPO_NAME} sources cloned"
    else
        message_error "unable to clone the ${REPO_NAME} sources"
    fi

    return ${RETURN_VALUE}
}

update_repository()
{
    REPO_NAME=${1}
    RETURN_VALUE=0

    clone_repository ${REPO_NAME}
    RETURN_VALUE=${?}

    if [ ${RETURN_VALUE} -eq 0 ] && [ -d ${REPOS_FOLDER}/${REPO_NAME} ]; then
        cd ${REPOS_FOLDER}/${REPO_NAME}
        case "${REPO_NAME}" in
            websocket)
                git pull
                RETURN_VALUE=${?}
                ;;
            boost)
                svn up
                RETURN_VALUE=${?}
                ;;
            *)
                RETURN_VALUE=1
                ;;
        esac
        cd -
    fi

    if [ ${RETURN_VALUE} -eq 0 ]; then
        message_info "${REPO_NAME} sources updated"
    else
        message_error "unable to update the ${REPO_NAME} sources"
    fi

    return ${RETURN_VALUE}
}

build_repository()
{
    REPO_NAME=${1}
    RETURN_VALUE=0

    update_repository ${REPO_NAME}
    RETURN_VALUE=${?}

    if [ ${RETURN_VALUE} -eq 0 ] && [ -d ${REPOS_FOLDER}/${REPO_NAME} ]; then
        cd ${REPOS_FOLDER}/${REPO_NAME}
        case "${REPO_NAME}" in
            websocket)
                cmake -DCMAKE_INSTALL_PREFIX:PATH=${PROJECTDIR}/lib \
                      -DWEBSOCKETPP_BOOST_LIBS:PATH=${PROJECTDIR}/lib/lib . && make install
                RETURN_VALUE=${?}
                ;;
            boost)
                ./bootstrap.sh --prefix=${PROJECTDIR}/lib && (./b2 || ./b2 install)
                RETURN_VALUE=${?}
                ;;
            *)
                RETURN_VALUE=1
                ;;
        esac
        RETURN_VALUE=${?}
	cd -
    fi

    if [ ${RETURN_VALUE} -eq 0 ]; then
        message_info "${REPO_NAME} sources built"
    else
        message_error "unable to build the ${REPO_NAME} sources"
    fi

    return ${RETURN_VALUE}
}

delete_repository()
{
    REPO_NAME=${1}
    RETURN_VALUE=1

    if [ -n ${REPO_NAME} -a -d ${REPOS_FOLDER}/${REPO_NAME} ]; then
        rm -r -f ${REPOS_FOLDER}/${REPO_NAME}
        if [ ${?} -eq 0 ]; then
            message_info "${REPO_NAME} sources removed"
            RETURN_VALUE=0
        else
            message_error "unable to delete the ${REPO_NAME} library sources"
        fi
    fi

    return ${RETURN_VALUE}
}

case "${COMMAND}" in
    init)
        if [ -n "${COMMAND_ARGUMENT}" ]; then
            clone_repository ${COMMAND_ARGUMENT}
            EXIT_VALUE=${?}
        else
            for repo in ${REPOS_LIST}; do
                clone_repository ${repo}
                EXIT_VALUE=${?}
                if [ ${EXIT_VALUE} -ne 0 ]; then break; fi
            done
        fi
        ;;
    update)
        if [ -n "${COMMAND_ARGUMENT}" ]; then
            update_repository ${COMMAND_ARGUMENT}
            EXIT_VALUE=${?}
        else
            for repo in ${REPOS_LIST}; do
                update_repository ${repo}
                EXIT_VALUE=${?}
                if [ ${EXIT_VALUE} -ne 0 ]; then break; fi
            done
        fi
        ;;
    build)
        if [ -n "${COMMAND_ARGUMENT}" ]; then
            build_repository ${COMMAND_ARGUMENT}
            EXIT_VALUE=${?}
        else
            for repo in ${REPOS_LIST}; do
                build_repository ${repo}
                EXIT_VALUE=${?}
                if [ ${EXIT_VALUE} -ne 0 ]; then break; fi
            done
        fi
        ;;
    delete)
        if [ -n "${COMMAND_ARGUMENT}" ]; then
            delete_repository ${COMMAND_ARGUMENT}
            EXIT_VALUE=${?}
        else
            for repo in ${REPOS_LIST}; do
                delete_repository ${repo}
                EXIT_VALUE=${?}
                if [ ${EXIT_VALUE} -ne 0 ]; then break; fi
            done
        fi
        ;;
    *)
        message_error "wrong command: '${COMMAND}\`"
        echo "Usage: ${SCRIPT_NAME} {init|update|build|delete} {boost|websocket}" >&1
        exit 1
    ;;
esac

if [ ${EXIT_VALUE} -ne 0 ]; then
    message_error "command ${COMMAND} failed"
fi

exit ${EXIT_VALUE}
