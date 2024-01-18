set(component_srcs "fontx.c"
	"ILI9486.c"
	"lcd_com.c"
    "8080_lcd_esp32s3.c"
	)

if(IDF_VERSION_MAJOR STREQUAL "4")
idf_component_register(SRCS "${component_srcs}"
                       PRIV_REQUIRES driver lvgl
                       PRIV_REQUIRES esp_adc_cal
                       INCLUDE_DIRS ".")
endif()

if(IDF_VERSION_MAJOR STREQUAL "5")
idf_component_register(SRCS "${component_srcs}"
                       PRIV_REQUIRES driver lvgl
                       PRIV_REQUIRES esp_adc
                       INCLUDE_DIRS ".")
endif()

