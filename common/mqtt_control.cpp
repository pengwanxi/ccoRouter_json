#include <string>
#include "globalbase.h"
#include "mqtt_control.h"
#include "cJSON.h"
#include "mosquitto.h"
#include "common_utils.h"

MqttControl::MqttControl()
{
	m_bConnect = false;
	m_callback = NULL;
}

MqttControl::~MqttControl()
{
	mosquitto_lib_cleanup();
}
bool MqttControl::mqttInit(MQTT_INFO &mqttinfo)
{
	m_logc = zlog_get_category("appcontrol");
	zlog_info(m_logc, "begin start mqtt_init ");

	m_mInfo = mqttinfo;

	int nRet = mosquitto_lib_init();
	if (nRet != MOSQ_ERR_SUCCESS)
	{
		zlog_info(m_logc, "mosquitto_lib_init fail err %d ", nRet);
		return false;
	}
	// m_mInfo.clientid
	m_mqttcco = mosquitto_new(NULL, true, NULL);
	if (!m_mqttcco)
	{
		zlog_error(m_logc, "create client failed..");
		mosquitto_lib_cleanup();
		return false;
	}

	nRet = mosquitto_username_pw_set(m_mqttcco, m_mInfo.user, m_mInfo.password);
	if (nRet != MOSQ_ERR_SUCCESS)
	{
		zlog_info(m_logc, "mosquitto_username_pw_set fail err %d ", nRet);
		return false;
	}

	// 程序意外结束
	/*
	std::string message = "{\"status\":\"offline\"}";
	std::string willTopic = "/ToPlatform/" + m_devSn +"/json/update/will";
	mosquitto_will_set(m_mqttcco, willTopic.c_str(), message.size(), message.c_str(), 0, false);
	*/

	setCallbackThis();

	mosquitto_message_callback_set(m_mqttcco, mqttMessageCallback);
	mosquitto_disconnect_callback_set(m_mqttcco, mqttDisconnectCallback);
	mosquitto_connect_callback_set(m_mqttcco, mqttConnectCallback);
	return true;
}

void MqttControl::setAppName(std::string appName)
{
	m_appname = appName;
}

void MqttControl::setCallbackThis()
{
	m_mqttcontrol = this;
}

bool MqttControl::mqttStart()
{
	int loop = mosquitto_loop_start(m_mqttcco);
	if (loop != MOSQ_ERR_SUCCESS)
	{
		zlog_error(m_logc, "mosquitto m_mqttiot loop error ");
		return false;
	}
	return true;
}
bool MqttControl::mqttConnect()
{
	int nCount = 0;

	while (true)
	{
		zlog_info(m_logc, "connect host %s port %d keeplive %d", m_mInfo.host, m_mInfo.nport, m_mInfo.nkeepalive);
		int nRet = mosquitto_connect(m_mqttcco, m_mInfo.host, m_mInfo.nport, m_mInfo.nkeepalive);
		if (nRet == MOSQ_ERR_SUCCESS)
		{
			zlog_info(m_logc, "connect success!");
			break;
		}
		zlog_info(m_logc, "connect failed!");
		sleep(1);
	}

	return true;
}

MqttControl *MqttControl::m_mqttcontrol = NULL;
void MqttControl::mqttMessageCallback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	m_mqttcontrol->mqttMessage(mosq, userdata, message);
}

void MqttControl::mqttDisconnectCallback(struct mosquitto *mosq, void *userdata, int connect)
{
	std::string topic = "disconnect";
	m_mqttcontrol->mqttConnect(mosq, topic, connect);
}

void MqttControl::mqttConnectCallback(struct mosquitto *mosq, void *userdata, int connect)
{
	std::string topic = "connect";
	m_mqttcontrol->mqttConnect(mosq, topic, connect);
}

void MqttControl::mqttMessage(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if (m_callback != NULL)
	{
		if (mosq == m_mqttcco)
		{
			m_callback(1, message->topic, (char *)message->payload);
		}
		else
		{
			zlog_debug(m_logc, "unknow mosq");
		}
	}
}

void MqttControl::mqttConnect(struct mosquitto *mosq, std::string topic, int connect)
{
	if (m_callback != NULL)
	{

		if (mosq == m_mqttcco)
		{
			std::string str = std::to_string(connect);
			m_callback(2, (char *)topic.c_str(), (char *)str.c_str());
		}
	}
}

bool MqttControl::mqttPublish(MQTT_SERVERTYPE type, const char *topic, int payloadlen, const void *payload, int qos, bool retain)
{
	int nRet;
	if (type == 1)
	{
		nRet = mosquitto_publish(m_mqttcco, NULL, topic, payloadlen, payload, qos, retain);
	}
	else
	{
		zlog_debug(m_logc, "unknow type ");
		return false;
	}

	if (nRet == MOSQ_ERR_SUCCESS)
	{
		return true;
	}
	else if (nRet == MOSQ_ERR_INVAL)
	{
		zlog_error(m_logc, "inpurt parameters were invalid!");
		return false;
	}
	else if (nRet == MOSQ_ERR_NOMEM)
	{
		zlog_error(m_logc, "out of memory condition occurred! ");
		return false;
	}
	else if (nRet == MOSQ_ERR_PROTOCOL)
	{
		zlog_error(m_logc, "protocol error communicating with the broker ");
		return false;
	}
	else if (nRet == MOSQ_ERR_PAYLOAD_SIZE)
	{
		zlog_error(m_logc, "the payload is too large ");
		return false;
	}
	else if (nRet == MOSQ_ERR_NO_CONN)
	{
		zlog_error(m_logc, "not connected! ");
		return false;
	}
	else
	{
		return false;
	}
}
bool MqttControl::mqttSubscribe(MQTT_SERVERTYPE type, int *mid, const char *topic, int qos)
{
	int nRet;
	if (type == 1)
	{
		nRet = mosquitto_subscribe(m_mqttcco, mid, topic, qos);
	}
	else
	{
		zlog_error(m_logc, "unknow type");
		return false;
	}

	if (nRet == MOSQ_ERR_SUCCESS)
	{
		return true;
	}
	else if (nRet == MOSQ_ERR_INVAL)
	{
		zlog_error(m_logc, "inpurt parameters were invalid! ");
		return false;
	}
	else if (nRet == MOSQ_ERR_NOMEM)
	{
		zlog_error(m_logc, "out of memory condition occurred! ");
		return false;
	}
	else if (nRet == MOSQ_ERR_NO_CONN)
	{
		// zlog_error(m_logc,"not connected! ");
		// bool connect = mqttConnect();
		// if(connect)
		// {
		// 	if(type == 0)
		// 	{
		// 		nRet = mosquitto_subscribe(m_mqttlocal, mid, topic, qos);
		// 	}else if (type == 1)
		// 	{
		// 		nRet = mosquitto_subscribe(m_mqttiot, mid, topic, qos);
		// 	}
		// 	if(nRet == MOSQ_ERR_SUCCESS)
		// 	{
		// 		return true;
		// 	}
		// }
		return false;
	}
	return false;
}

void MqttControl::setCallback(void (*callback)(int, char *, char *))
{
	m_callback = callback;
}
