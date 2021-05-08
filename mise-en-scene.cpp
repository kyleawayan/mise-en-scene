#include <iostream>
// vcpkg install cpprestsdk[default-features,websockets]:x64-windows
#include <cpprest/ws_client.h>
#include "Keyboard.h"
#include "MakeJSONs.h"

using namespace std;
using namespace web;
using namespace web::websockets::client;

wstring convertStringToWString(string string)
{
    return wstring(string.begin(), string.end());
}

int main()
{
    wstring wsUrl;
    wstring roomId;
    wstring username;

    cout << "Enter ws URL: ";
    string wsString;
    getline(cin, wsString);
    wsUrl = convertStringToWString(wsString);

    websocket_client client;
    client.connect(wsUrl).then([]() { cout << "Conntected to Mise-en-Scene server" << endl; }).wait();

    cout << "Enter room ID: ";
    string roomIdString;
    getline(cin, roomIdString);
    roomId = convertStringToWString(roomIdString);

    cout << "Enter a username: ";
    string usernameString;
    getline(cin, usernameString);
    username = convertStringToWString(usernameString);

    utility::stringstream_t joinJson = createJoinJson(roomId, username);

    // cout << str(joinJson) << endl;

    // websocket_outgoing_message msg;
    // msg.set_utf8_message(joinJson);
    // client.send(joinJsoin).then([]() { /* Successfully sent the message. */ }).wait();

    // hitKey(0xB3);
}