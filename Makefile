all: quickstart.c
	gcc quickstart.c -lX11 -lcrypt -DHAVE_SHADOW_H && sudo chown root a.out && sudo chmod 4755 a.out
