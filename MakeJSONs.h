#pragma once
#ifndef MAKEJSONS
#define MAKEJSONS

#include <cpprest/json.h>



utility::stringstream_t createJoinJson(std::wstring roomId, std::wstring username)
{
	utility::stringstream_t stream;
	web::json::value joinObject = web::json::value::object();
	web::json::value action = web::json::value::string(U("join"));

	web::json::value payloadObject = web::json::value::object();
	payloadObject[U("roomId")] = web::json::value::string(roomId);
	payloadObject[U("username")] = web::json::value::string(username);

	joinObject[U("action")] = action;
	joinObject[U("payload")] = payloadObject;

	joinObject.serialize(stream);
	return stream;
}

utility::stringstream_t createRoomJson(std::wstring roomId)
{
	utility::stringstream_t stream;
	web::json::value joinObject = web::json::value::object();
	web::json::value action = web::json::value::string(U("create"));

	web::json::value payloadObject = web::json::value::object();
	payloadObject[U("roomId")] = web::json::value::string(roomId);

	joinObject[U("action")] = action;
	joinObject[U("payload")] = payloadObject;

	joinObject.serialize(stream);
	return stream;
}

utility::stringstream_t createPlaybackActionJson(std::wstring roomId, std::wstring username)
{
	utility::stringstream_t stream;
	web::json::value joinObject = web::json::value::object();
	web::json::value action = web::json::value::string(U("playbackToggle"));

	web::json::value payloadObject = web::json::value::object();
	payloadObject[U("roomId")] = web::json::value::string(roomId);
	payloadObject[U("username")] = web::json::value::string(username);

	joinObject[U("action")] = action;
	joinObject[U("payload")] = payloadObject;

	joinObject.serialize(stream);
	return stream;
}

#endif