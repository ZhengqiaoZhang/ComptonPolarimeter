#include <cmath> // for cos, sin, atan2
#include <iostream> // for std::cout

// 定义一个结构体来存储二维点的坐标
struct Point {
    double x, y;
};

// 函数以起点（x1, y1），方向theta1（以弧度为单位），和半径R0为输入，输出圆心的坐标
Point findCircleCenter(double x1, double y1, double theta1, double R0) {
    Point center;
    center.x = x1 + R0 * cos(theta1);  //depends on bending angle
    center.y = y1 - R0 * sin(theta1);
    return center;
}

// 函数以圆心（xc, yc），起点（x1, y1），半径R0，和旋转角theta2（以弧度为单位）为输入，输出旋转后的坐标
Point rotatePointAroundCenter(double xc, double yc, double x1, double y1, double R0, double theta2) {
    // 计算起始点相对于圆心的角度
    double alpha = atan2(y1 - yc, x1 - xc);
    
    // 计算新的角度
    double beta = alpha + theta2;

    // 计算新的坐标
    Point newPoint;
    newPoint.x = xc + R0 * cos(beta); 
    newPoint.y = yc + R0 * sin(beta);

    return newPoint;
}

void calXY_circle() {
    //double x1 = 0.3440665993, y1 = 93.27935437; // 起始点
    	double x1 = -589.52931329013,y1=30.2903035053;
    double theta1 = -3.1335926535898;//M_PI / 4; // 初始方向（以弧度为单位）
    double R0 = 0.89140008356876/0.0015; // 半径
    double theta2 = 0.0015*0.5; // 旋转角度（以弧度为单位）  (depends on the bending angle)

    // 计算圆心
    Point center = findCircleCenter(x1, y1, theta1, R0);

    // 输出圆心坐标
    std::cout << "The circle center is at (" << center.x << ", " << center.y << ")" << std::endl;

    // 计算旋转后的坐标
    Point newPoint = rotatePointAroundCenter(center.x, center.y, x1, y1, R0, theta2);

    // 输出旋转后的坐标
    std::cout << "The new point is at (" << std::setprecision(10) <<newPoint.x << ", " << newPoint.y << ")" << std::endl;

    return 0;
}

