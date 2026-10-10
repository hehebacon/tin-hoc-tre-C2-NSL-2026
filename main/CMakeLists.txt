idf_component_register(
    SRCS
        "app_main.cpp"
        "control/sorter_controller.cpp"
        "actuators/actuator_manager.cpp"
        "perception/classifier.cpp"
    INCLUDE_DIRS "." "control" "actuators" "perception"
    REQUIRES driver
)
