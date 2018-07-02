/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

#include "vm_task.h"
#include "communication_task.h"

extern SemaphoreHandle_t xSemaphore;

enum main_status_type
{
    main_init_1,
	main_init_2,
	main_init_3,
	main_init_4,
	main_user_input_1,
	main_user_input_2,
	main_user_input_3,
	main_user_input_4,
	main_balance_check,
	main_recommand,
	main_purchase_choice,
	main_purchase_confirm
};

void _Main_Enqueue (int target_id, vm_data _data)
{
    _data.source_id = id_main;
    _data.target_id = target_id;
    enQueue(_data);
}

void _Wifi_Main_Enqueue (WIFI_data _wifi_data)
{
	vm_data _data;
	_data.source_id   = id_main;
	_data.target_id   = id_wifi;
	strncpy(_data.user, _wifi_data.user, 4);
	_data.target_item = _wifi_data.target_item;

	for ( int i = 0; i < 4; i++ )
			_data.body[i].i = _wifi_data.body[i].i;

	for ( int i = 0; i < 4; i++ )
	{
		strncpy(_data.type, _wifi_data.type[i], 6);
		strncpy(_data.name, _wifi_data.name[i], 6);
		_Main_Enqueue(id_wifi, _data);
	}
}

int main(int argc, char **argv)
{
	EMBARC_PRINTF("entering main function\r\n");
	// 實作各 main
	// 流程都寫在這裡

    vm_data data = {0}; // data for receiveing
	WIFI_data local_data = {0}; // local database
	int main_status = main_user_input_1;
	char output = 'Z';

	while (1)
	{
		if (main_status == main_init_1)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						strncpy(local_data.type[data.target_item], data.type, 6);
						strncpy(local_data.name[data.target_item], data.name, 6);
						local_data.body[data.target_item].i = data.body[data.target_item].i;
						local_data.source_id = id_main;
						main_status = main_init_2;
					}
				}
			}
		}
		if (main_status == main_init_2)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						strncpy(local_data.type[data.target_item], data.type, 6);
						strncpy(local_data.name[data.target_item], data.name, 6);
						local_data.body[data.target_item].i = data.body[data.target_item].i;
						local_data.source_id = id_main;
						main_status = main_init_3;
					}
				}
			}
		}
		if (main_status == main_init_3)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						strncpy(local_data.type[data.target_item], data.type, 6);
						strncpy(local_data.name[data.target_item], data.name, 6);
						local_data.body[data.target_item].i = data.body[data.target_item].i;
						local_data.source_id = id_main;
						main_status = main_init_4;
					}
				}
			}
		}
		if (main_status == main_init_4)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						strncpy(local_data.type[data.target_item], data.type, 6);
						strncpy(local_data.name[data.target_item], data.name, 6);
						local_data.body[data.target_item].i = data.body[data.target_item].i;
						local_data.source_id = id_main;
						main_status = main_user_input_1;
					}
				}
			}
		}
		if (main_status == main_user_input_1)
		{
			EMBARC_PRINTF("main reading from numpad started.============================================= \r\n");
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						EMBARC_PRINTF("receive char: %c\n", (char) data.body[0].i);
						data.user[0] = (char) data.body[0].i;
						data.status  = user_input_1;
						data.target_id = no_item;
						_Main_Enqueue(id_oled, data);
						local_data.user[0] = (char) data.body[0].i;
						main_status = main_user_input_2;
					}
				}
			}
		}
		if (main_status == main_user_input_2)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						EMBARC_PRINTF("receive char: %c\n", (char) data.body[0].i);
						data.user[1] = (char) data.body[0].i;
						data.status  = user_input_2;
						data.target_id = no_item;
						_Main_Enqueue(id_oled, data);
						local_data.user[1] = (char) data.body[0].i;
						main_status = main_user_input_3;
					}
				}
			}
		}
		if (main_status == main_user_input_3)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						EMBARC_PRINTF("receive char: %c\n", (char) data.body[0].i);
						data.user[2] = (char) data.body[0].i;
						data.status  = user_input_3;
						data.target_id = no_item;
						_Main_Enqueue(id_oled, data);
						local_data.user[2] = (char) data.body[0].i;
						main_status = main_user_input_4;
					}
				}
			}
		}
		if (main_status == main_user_input_4)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						EMBARC_PRINTF("receive char: %c\n", (char) data.body[0].i);
						data.user[3] = (char) data.body[0].i;
						data.status  = user_input_4;
						data.target_id = no_item;
						data.status    = balance_check;
						_Main_Enqueue(id_oled, data);
						local_data.user[3] = (char) data.body[0].i;
						
						local_data.target_item = no_item;
						local_data.status      = balance_check;
						_Wifi_Main_Enqueue(local_data);

						// check balance just once
						local_data.status = server_do_nothing;
						_Wifi_Main_Enqueue(local_data);

						EMBARC_PRINTF("user input: %s", data.user);
						main_status = main_user_input_1;
						// main_status = main_balance_check;
					}
				}
			}
		}
		if (main_status == main_balance_check)
		{

			_Wifi_Main_Enqueue(local_data);
			main_status = main_recommand;
		}
		if (main_status == main_recommand)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						data.status = sell_recommand;
						_Main_Enqueue(id_oled, data);
						main_status = main_purchase_choice;
					}
				}
			}
		}
		if (main_status == main_purchase_choice)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						data.status = purchase_check;
						local_data.target_item = data.body[0].i;
						_Main_Enqueue(id_oled, data);
						main_status = main_purchase_confirm;
					}
				}
			}
		}
		if (main_status == main_purchase_confirm)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						if ((char) data.body[0].i == '*')
						{
							data.target_item = local_data.target_item;

							for ( int i = 0; i < 4; i++ )
								data.body[i].i = 0;
							
							data.body[local_data.target_item].i = 1;
							local_data.status = purchase_confirm;
							local_data.body[local_data.target_item].i--;
							_Main_Enqueue(id_dcmotor, data);
							_Wifi_Main_Enqueue(local_data);

							// to avoid purchase record sent by wifi from being more than once
							local_data.status = server_do_nothing;
							_Wifi_Main_Enqueue(local_data);
						}

						local_data.user[0] = 0;
						local_data.user[1] = 0;
						local_data.user[2] = 0;
						local_data.user[3] = 0;
						main_status = main_user_input_1;
					}
				}
			}
		}
		vTaskDelay(20);
	}

	return E_SYS;
}
