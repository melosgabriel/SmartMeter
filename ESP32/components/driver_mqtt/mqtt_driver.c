#include "mqtt_driver.h"

static const char *TAG = "MQTT_EXAMPLE";
static esp_mqtt_client_handle_t m_MqttClient;
static EventGroupHandle_t s_mqtt_event_group;
/**
 * In case the is a bug it is probably because the event group is deleted while
 * the mqtt handler cb continues to work. In case of a disconnection or a reconnection
 * reboot the device, at least for now.
 */
static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    char sMqttTopic[80];
    char sMqttMessage[80];

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            
            sprintf(sMqttTopic, "/ziggy/default"); //! TODO: CHANGE TO /smartmeter/default
            sprintf(sMqttMessage, "Meter %d is now connected to building %s", CONFIG_METER_NUMBER, CONFIG_METER_BUILDING_NAME);

            msg_id = esp_mqtt_client_publish(client, sMqttTopic, sMqttMessage, 0, 1, 0);
            xEventGroupSetBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            xEventGroupSetBits(s_mqtt_event_group, MQTT_FAIL_BIT);
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}
/**
 * In this function we have to wait for the connection otherwise it is possible that we start publishing
 * BEFORE we are actually connected. This can lead to a timeout in case of QoS>0 and a reboot loop.
 * 4 hours lost to figure that out :<
 */
void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL,
    };

    m_MqttClient = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(m_MqttClient, ESP_EVENT_ANY_ID, mqtt_event_handler, m_MqttClient);
    esp_mqtt_client_start(m_MqttClient);
    
    s_mqtt_event_group = xEventGroupCreate();
    EventBits_t bits = xEventGroupWaitBits(s_mqtt_event_group,
            MQTT_CONNECTED_BIT | MQTT_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);
    
    if (bits & MQTT_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to MQTT broker: %s", CONFIG_BROKER_URL);
    } else if (bits & MQTT_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to MQTT broker: %s", CONFIG_BROKER_URL);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
    vEventGroupDelete(s_mqtt_event_group);

}

void mqtt_publish(char * sMqttTopic, char * sMqttMessage, int len, int qos, int retain)
{
    esp_mqtt_client_publish(m_MqttClient, sMqttTopic, sMqttMessage, len, qos, retain);
}