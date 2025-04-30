/*
 *   This is a simple example in C of using the rich presence API asynchronously.
 */


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "discord_rpc.h"
#include "Rogue.h"
#include "GlobalsBase.h"

static const char* APPLICATION_ID = "1336382837049393294";

static int64_t StartTime;
static int SendPresence = 1;

void updateDiscordPresence() {
    if (SendPresence) {
        char buffer[256];
        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        discordPresence.details = gameConst->versionString;
        sprintf(buffer, "On depth %d/%d%s", rogue.depthLevel, gameConst->amuletLevel, numberOfMatchingPackItems(AMULET, 0, 0, false) ? ", amulet in hand" : "");
        discordPresence.state = buffer;
        discordPresence.startTimestamp = StartTime;
        discordPresence.endTimestamp = time(0);
        discordPresence.largeImageKey = "icon";
        discordPresence.smallImageKey = "";
        discordPresence.instance = 0;
        Discord_UpdatePresence(&discordPresence);
    }
    else {
        Discord_ClearPresence();
    }
}

static void handleDiscordReady(const DiscordUser* connectedUser)
{
    printf("\nDiscord: connected to user %s#%s - %s\n",
           connectedUser->username,
           connectedUser->discriminator,
           connectedUser->userId);
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
    printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
    printf("\nDiscord: error (%d: %s)\n", errcode, message);
}
/*
static void handleDiscordJoin(const char* secret)
{
    printf("\nDiscord: join (%s)\n", secret);
}

static void handleDiscordSpectate(const char* secret)
{
    printf("\nDiscord: spectate (%s)\n", secret);
}

static void handleDiscordJoinRequest(const DiscordUser* request)
{
    int response = -1;
    char yn[4];
    printf("\nDiscord: join request from %s#%s - %s\n",
           request->username,
           request->discriminator,
           request->userId);
    do {
        printf("Accept? (y/n)");
        if (!prompt(yn, sizeof(yn))) {
            break;
        }

        if (!yn[0]) {
            continue;
        }

        if (yn[0] == 'y') {
            response = DISCORD_REPLY_YES;
            break;
        }

        if (yn[0] == 'n') {
            response = DISCORD_REPLY_NO;
            break;
        }
    } while (1);
    if (response != -1) {
        Discord_Respond(request->userId, response);
    }
}
*/
void discordInit() {
    DiscordEventHandlers handlers;
    printf("Discord init!\n");
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;
    //handlers.joinGame = handleDiscordJoin;
    //handlers.spectateGame = handleDiscordSpectate;
    //handlers.joinRequest = handleDiscordJoinRequest;
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
    StartTime = time(0);
    SendPresence = 1;
}

void discordUpdate() {
    updateDiscordPresence();
    #ifdef DISCORD_DISABLE_IO_THREAD
    Discord_UpdateConnection();
    #endif
    Discord_RunCallbacks();;
    printf("Discord update!\n");
}

void discordShutdown() {
    Discord_ClearPresence();
    Discord_Shutdown();
    SendPresence = 0;
    printf("Discord shutdown!\n");
}

/*
void gameLoop() {
    char line[512];
    char* space;

        #ifdef DISCORD_DISABLE_IO_THREAD
        Discord_UpdateConnection();
        #endif
        Discord_RunCallbacks();
    }
}*/
