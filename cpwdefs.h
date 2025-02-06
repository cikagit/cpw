#ifndef __CPWDEFS_H
#define __CPWDEFS_H

// Valores Minimos y maximos
#define DATE_MIN	CTime(1971,1,1,0,0,0)
#define DATE_MAX	CTime(2038,1,1,0,0,0)
//Fichero de cambio
#define TK_FILE_CAMBIO "C:\\cpw\\InfCamDia.txt"

//COMANDOS
#define	TK_CMD_DIARIO "Diario"
#define TK_CMD_MEZCLAS "Mezclas"
#define TK_CMD_MOLDES "Moldes"
#define TK_CMD_OFART "OFs y Articulos"
#define TK_CMD_PEDIDOS "Pedidos"
// Opciones
#define TK_OPC_ORIENTACION	'H'

// Tamaños
//#define TK_SIZE_CODART	8
#define TK_LIM_ERRSTRUCT 3

// Errores de OM
#define TK_OMERR_CONFMOLDE	1
#define TK_OMERR_CONFMOLDEELEM 2

// Constantes de Cache
#define TK_CACHE_LOCALES	20
#define TK_CACHE_ARTIC		500
#define TK_CACHE_MOLDES		500
#define TK_CACHE_ACABADO	1000
#define TK_CACHE_MAQ		50
#define TK_CACHE_ORDMAQ		4000
#define TK_CACHE_CAMBMOLD	1000
#define TK_CACHE_MEZCLAS	400
#define TK_CACHE_INACT		50


// Numero de Brushes en matriz

#define CPW_N_BRUSHES	3

#define TK_COLOR_0	12632256
#define TK_COLOR_1	33023
#define TK_COLOR_2	658160

// Valores por defecto

#define TK_ESCALA_DEF 300
#define TK_ADJUST_INACT_SECS 300
#define TK_INACT_DEF  (100.0 / 23.0)

#define TK_LOTE_DEF 120
#define TK_DEL_MINSECS	18000L

#define TK_SEGS_MIN_LIS	60

// Conversión entre fechas de Paradox y Access

#define TK_CONV_DATE		2415019L

// Constantes que definen los tipos de OrdMaqs

#define	TK_ORD_NORMAL		0x0000
#define	TK_ORD_INACT		0x0001

#define	TK_ORD_MOVIL		0x01
#define	TK_ORD_FIJA			0x02
#define	TK_ORD_HUECO		0x04

#define	TK_FIJA_FECINI		0x01
#define	TK_FIJA_FECFIN		0x02
#define	TK_FIJA_TIEMPO		0x04

// Numero de dias que hay que adelantar al verificar las ordenes
#define	TK_NUMDIAS_VERIF	7

// Segundos que hay en un dia y una hora
#define TK_SEGS_DIA		86400L
#define TK_SEGS_HORA	3600L
#define TK_SEGS_TURNO	28800L
#define TK_CAMBIO_DIA	16800L

// Puntos de las cabeceras de la vista
#define TK_POINTS_HDR	18
#define TK_POINTS_COL	25

// Valores de peligro
#define TK_PELIGRO_NO	0
#define TK_PELIGRO_INT	1
#define TK_PELIGRO_MAX	2

// Valores por defecto

#define TK_DEF_X	1
#define TK_DEF_Y	1
//#define TK_DEF_W	125
#define TK_DEF_H	20
#define TK_DEF_SEP	20
#define TK_DEF_MARGEN  57600L
#define TK_DEF_FONT	"Fixedsys"
#define TK_DEF_FONTH	 -10
//#define TK_DEF_NTURNOS	 2
//#define TK_NUM_TURNOS	3
#define TK_NUM_TURNOS_MAX	3

// Edicion de calendario

#define TK_EDITCAL_W		15
#define TK_EDITCAL_XDESP 	0


// Segundos que representa un punto de la pantalla, por defecto
#define TK_DEF_SEGS_PUNTO	600	 


// minutos que se desprecian si pasan de la hora
#define TK_MINUTOS_DESPREC	5

//#define TK_TURNO1	1
//#define TK_TURNO2 	2
//#define TK_TURNO3	3

//#define TK_TURNO1_FIN 	14
//#define TK_TURNO2_FIN	22
//#define TK_TURNO3_FIN	6


// Codigos de empresa
#define TK_EMPRESA_BERRIATUA 1

// Códigos de error
#define TK_ERROR_KEY	"*ERR"

// Máquina no encontrada
#define TK_ERR_MAQNOTFOUND		101
// OrdMaq no insertado en ninguna ProgMaq
#define TK_ERR_ORDMAQNOTINSERT	102

// gfh
#define TK_GFH_ACABADO		2   	
#define TK_GFH_INYECCION			1

// valores maximos
#define TK_MAX_HUECO	0xFFFFFFFFL
#define TK_MAX_MARGEN 28800L
// valores minimos
#define TK_MIN_PROG		12


#define CM_ORDEN_DATOS		501
#define CM_ORDEN_MOVER     502
#define CM_ORDEN_BORRAR    503
#define CM_FIJA_FECINI     504
#define CM_FIJA_TIEMPO     505
#define CM_MAQ_SELECTALL   506
#define CM_MAQ_RECALC      507
#define CM_MAQ_COMPACTAR   508
#define CM_MAQ_INFO		   509
#define CM_CREAR_INACT		510
#define CM_ALTER_FECINI		511
#define CM_ALTER_TIEMPO		512
#define CM_ALTER_CANT		513
#define CM_ALTER_FECENT		514
#define CM_MAQ_IMPRIMIR		515
#define CM_ORDEN_CREINACT  516
#define CM_CALC_TIEMPO	  517
#define CM_MAQ_EDITCAL	   518
#define CM_ALTER_PERSONAS  519
#define CM_ALTER_ORDFAB		520
#define CM_ORDEN_SELECFIN  521
#define CM_ALTER_BLOQUE		522

#define TK_MOLDE_ELEM_INYECTORA	"Iny"
#define TK_MOLDE_ELEM_SOPLADO	"Sop"

#endif // __CPWDEFS_H
