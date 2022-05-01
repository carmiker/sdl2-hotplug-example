/*
BSD Zero Clause License

Copyright (c) 2022 Rupert Carmichael

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
*/

// cc sdl2hp.c -std=c99 `sdl2-config --cflags --libs` -o sdl2hp

#include <stdio.h>
#include <SDL.h>

#define MAXPORTS 4

static SDL_Joystick *joystick[MAXPORTS];
static int jsports[MAXPORTS];
static int jsiid[MAXPORTS];

int main(int argc, char *argv[]) {
    SDL_Event event;
    int running = 1;

    SDL_Init(SDL_INIT_JOYSTICK);

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    running = 0;
                    break;
                }
                case SDL_JOYDEVICEADDED: {
                    int port = 0;

                    // Choose next unplugged port
                    for (int i = 0; i < MAXPORTS; ++i) {
                        if (!jsports[i]) {
                            joystick[i] = SDL_JoystickOpen(event.jdevice.which);
                            SDL_JoystickSetPlayerIndex(joystick[i], i);
                            jsports[i] = 1;
                            jsiid[i] = SDL_JoystickInstanceID(joystick[i]);
                            port = i;
                            break;
                        }
                    }

                    printf("Joystick Connected: %s\n"
                        "\tInstance ID: %d, Player: %d\n",
                        SDL_JoystickName(joystick[port]),
                        jsiid[port],
                        SDL_JoystickGetPlayerIndex(joystick[port])
                    );

                    printf("Ports: %d %d %d %d\n\n",
                        jsports[0], jsports[1], jsports[2], jsports[3]
                    );

                    break;
                }
                case SDL_JOYDEVICEREMOVED: {
                    int id = event.jdevice.which;
                    printf("Instance ID: %d\n", id);
                    for (int i = 0; i < MAXPORTS; ++i) {
                        // If it's the one that got disconnected...
                        if (jsiid[i] == id) {
                            jsports[i] = 0; // Notify that this is unplugged
                            printf("Joystick %d Disconnected\n", i);
                            SDL_JoystickClose(joystick[i]);
                            break;
                        }
                    }
                    printf("Ports: %d %d %d %d\n\n",
                        jsports[0], jsports[1], jsports[2], jsports[3]
                    );
                    break;
                }
            }
        }
    }

    return 0;
}
