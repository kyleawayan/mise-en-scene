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

    websocket_callback_client client;
    client.connect(wsUrl).then([]() { cout << "Conntected to Mise-en-Scene server" << endl; }).wait();

    cout << "Would you like to create a room? (y/n) ";
    string createRoom;
    getline(cin, createRoom);

    if (createRoom == "y")
    {
        // to do: make these functions
        int roomIdInt = get_random_int(1000, 9999);
        roomId = to_wstring(roomIdInt);
        utility::stringstream_t createJson = createRoomJson(roomId);
        wstring createWString = createJson.str();

        websocket_outgoing_message sendToSocketJoinJson;
        sendToSocketJoinJson.set_utf8_message(wstring_to_utf8(createWString));
        client.send(sendToSocketJoinJson).then([roomIdInt]() { cout << "Creating room " << roomIdInt << endl; }).wait();
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

    client.set_message_handler([](websocket_incoming_message msg)
    {
            return msg.extract_string().then([](string body) {
                json::value jsonMessage = json::value::parse(convertStringToWString(body));
                auto action = jsonMessage.at(U("action"));
                auto message = jsonMessage.at(U("message"));

                if (wstring_to_utf8(action.as_string()) == "playbackToggle")
                {
                    hitKey(0xB3);
                }

                cout << wstring_to_utf8(message.as_string()) << endl;
            });
    });

    ThrottledHitKey delKey(0x2E, 2);

    const utility::stringstream_t playbackToggleJson = createPlaybackActionJson(roomId, username);
    const wstring playbackToggleJsonWString = playbackToggleJson.str();

    while (true)
    {
        if (GetKeyState(VK_END) & 0x8000)
        {
            bool shouldSend = delKey.throttle();
            if (shouldSend)
            {
                websocket_outgoing_message sendToSocketPlaybackAction;
                sendToSocketPlaybackAction.set_utf8_message(wstring_to_utf8(playbackToggleJsonWString));
                client.send(sendToSocketPlaybackAction).wait();
            }
        }
    }
}