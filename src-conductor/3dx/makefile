# ============================================================
# MAKEFILE to compile the developer files on a IBM Workstation
# ============================================================
# Copyright (C) 1984, 2002 3Dconnexion GmbH / Inc.
# An der Hartmuehle 8
# D - 82229 Seefeld
# mailto:development@3dconnexion.com
# ============================================================

all: 		xapp
		ls -al xapp

xdrvlib.o:	xdrvlib.c xdrvlib.h
		cc xdrvlib.c -c -Wall -DIBM

xapp:		xapp.c xdrvlib.o
		cc xapp.c xdrvlib.o -o xapp -lX11 -lm -Wall -DIBM

