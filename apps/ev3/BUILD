load("@com_nvidia_isaac//engine/build:isaac.bzl", "isaac_app", "isaac_subgraph")

isaac_subgraph(
    name = "ev3_hardware_subgraph",
    modules = [
        "@com_nvidia_isaac//packages/ydlidar",
        "ev3"
    ],
    subgraph = "ev3_hardware.subgraph.json",
    visibility = ["//visibility:public"],
)

isaac_subgraph(
    name = "2d_ev3_subgraph",
    data = [
        ":ev3_hardware_subgraph",
        "@com_nvidia_isaac//packages/navigation/apps:differential_base_navigation_subgraph",
        "@com_nvidia_isaac//packages/navigation/apps:goal_generators_subgraph",
        "@com_nvidia_isaac//packages/navigation/apps:differential_base_commander_subgraph"
    ],
    modules = [],
    subgraph = "2d_ev3.subgraph.json",
    visibility = ["//visibility:public"],
)

isaac_app(
    name = "ev3",
    data = [
        ":2d_ev3_subgraph"
    ],
    modules = [
        "@com_nvidia_isaac//packages/navigation",
        "@com_nvidia_isaac//packages/perception",
        "@com_nvidia_isaac//packages/planner",
        "@com_nvidia_isaac//packages/sensors:joystick",
        "@com_nvidia_isaac//packages/ydlidar",
    ],
)


isaac_app(
    name = "joystick",
    data = [
        ":2d_ev3_subgraph",
        "@com_nvidia_isaac//packages/navigation/apps:differential_base_commander_subgraph",
        "@com_nvidia_isaac//packages/navigation/apps:differential_base_imu_odometry_subgraph",
        "@com_nvidia_isaac//packages/navigation/apps:local_map_subgraph",
    ],
    modules = [
        "@com_nvidia_isaac//packages/navigation",
        "@com_nvidia_isaac//packages/perception",
        "@com_nvidia_isaac//packages/planner",
        "@com_nvidia_isaac//packages/sensors:joystick",
        "@com_nvidia_isaac//packages/ydlidar",
        "ev3",
    ],
)

isaac_app(
    name = "gmapping_distributed_host",
    modules = [
        "@com_nvidia_isaac//packages/lidar_slam:g_mapping",
    ],
)

isaac_app(
    name = "gmapping_distributed_ev3",
    data = [
        ":2d_ev3_subgraph",
        "@com_nvidia_isaac//packages/navigation/apps:differential_base_commander_subgraph",
        "@com_nvidia_isaac//packages/navigation/apps:differential_base_imu_odometry_subgraph",
        "@com_nvidia_isaac//packages/navigation/apps:local_map_subgraph",
    ],
    modules = [
        "@com_nvidia_isaac//packages/navigation",
        "@com_nvidia_isaac//packages/perception",
        "@com_nvidia_isaac//packages/planner"
    ],
)

