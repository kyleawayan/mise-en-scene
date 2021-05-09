#include <iostream>
#include <codecvt>
#include <string>
// vcpkg install cpprestsdk[default-features,websockets]:x64-windows
#include <cpprest/ws_client.h>
#include "Keyboard.h"
#include "MakeJSONs.h"

using namespace std;
using namespace web;
using namespace web::websockets::client;

random_device rd;
mt19937 rng(rd());

int get_random_int(int min, int max)
{
    uniform_int_distribution<int> uni(min, max);
    return uni(rng);
}

wstring convertStringToWString(string string)
{
    return wstring(string.begin(), string.end());
}

// https://stackoverflow.com/questions/4358870/convert-wstring-to-string-encoded-in-utf-8/12903901
string wstring_to_utf8(const std::wstring& str)
{
    wstring_convert<codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
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

    cout << "Would you like to create a room? (y/n) ";
    string createRoom;
    getline(cin, createRoom);

    if (createRoom == "y")
    {
        // to do: make these functions
        roomId = to_wstring(get_random_int(1000, 9999));
        utility::stringstream_t createJson = createRoomJson(roomId);
        wstring createWString = createJson.str();

        websocket_outgoing_message sendToSocketJoinJson;
        sendToSocketJoinJson.set_utf8_message(wstring_to_utf8(createWString));
        client.send(sendToSocketJoinJson).then([]() { cout << "Creating room..." << endl; }).wait();
    }
    else {
        cout << "Enter room ID: ";
        string roomIdString;
        getline(cin, roomIdString);
        roomId = convertStringToWString(roomIdString);
    }

    cout << "Enter a username: ";
    string usernameString;
    getline(cin, usernameString);
    username = convertStringToWString(usernameString);

    utility::stringstream_t joinJson = createJoinJson(roomId, username);
    wstring joinWString = joinJson.str();

    websocket_outgoing_message sendToSocketJoinJson;
    sendToSocketJoinJson.set_utf8_message(wstring_to_utf8(joinWString));
    client.send(sendToSocketJoinJson).then([]() { cout << "Joining room..." << endl; }).wait();



    // hitKey(0xB3);
}