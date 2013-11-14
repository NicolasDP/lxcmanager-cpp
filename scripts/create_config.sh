#!/bin/sh

#
## create_config.sh for Gabyni
##
## Made by Nicolas DI PRIMA
## Login   <nicolas@di-prima.fr>
##
## Started on  Fri 23 Aug 2013 12:37:25 BST Nicolas DI PRIMA
## Last update Sat 14 Sep 2013 14:15:39 BST Nicolas DI PRIMA
#

PROJECTNAME=${1}
PROJECTVERSION=${2}
PROJECTSUBVERSION=${3}
PROJECTREVISION=${4}

CONFIG_HEADER_FILE=${5}


cat << EOF > ${CONFIG_HEADER_FILE}
/* Auto generated file, DO NOT EDIT
 * date: `date` */

#define PROJECT_NAME           "${PROJECTNAME}"

#define PROJECT_VERSION        ${PROJECTVERSION}
#define PROJECT_SUBVERSION     ${PROJECTSUBVERSION}
#define PROJECT_REVISION       ${PROJECTREVISION}

#define PROJECT_VERSION_STRING "${PROJECTVERSION}.${PROJECTSUBVERSION}-${PROJECTREVISION}"
EOF
