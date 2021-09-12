#include "client.h"

void leer_consola_con_accion(void (*hacer_algo)(char*));

int main(void) {
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");
	config = iniciar_config();

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor
	ip = config_get_string_value(config, "IP");
	log_info(logger, "El valor de la IP: %s", ip);
	puerto = config_get_string_value(config, "PUERTO");
	log_info(logger, "El valor del Puerto: %s", puerto);
	valor = config_get_string_value(config, "CLAVE");
	log_info(logger, "El valor de la CLAVE: %s", valor);
	//Loggear valor de config

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.
	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	//enviar CLAVE al servirdor
	enviar_mensaje(valor, conexion);

	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void) {
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "tp0", 1, LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void) {
	t_config* nuevo_config;
	nuevo_config = config_create("config/tp0.config");
	return nuevo_config;
}

void leer_consola(t_log* logger) {
	void loggear(char* string) {
		log_info(logger, string);
	}

	leer_consola_con_accion(loggear);

}

void paquete(int conexion) {
	//Ahora toca lo divertido!

	t_paquete* paquete = crear_super_paquete();

	void agregar_string_paquete(char* string) {
		agregar_a_paquete(paquete,string, strlen(string) + 1);
	}

	leer_consola_con_accion(agregar_string_paquete);

	enviar_paquete(paquete, conexion);

	eliminar_paquete(paquete);
}

void leer_consola_con_accion(void (*hacer_algo)(char*)) {
	char* leido;

	//El primero te lo dejo de yapa
	leido = readline(">");

	while (strncmp(leido, "", 1) != 0) {
		hacer_algo(leido);
		free(leido);
		leido = readline(">");
	}

	free(leido);
}

void terminar_programa(int conexion, t_log* logger, t_config* config) {
	log_info(logger, "Terminando programa");
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
