#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include "GameObjects.h"
#include "Leaderboard.h"

using namespace std;

struct F {
    F() : lb( 0 ) { 
		BOOST_TEST_MESSAGE( "setup fixture" );
	}
    ~F()         { 
		BOOST_TEST_MESSAGE( "teardown fixture" ); 
	}

    Leaderboard *lb;
	ProjectileType *prjt;
	Projectile *prj;
	Unit *body;
};

//____________________________________________________________________________//

BOOST_FIXTURE_TEST_SUITE(s, F)

// тест - создание новой таблицы рекордов с пустым указателем на имя файла
BOOST_AUTO_TEST_CASE(test_case1) {
    BOOST_CHECK_THROW (new Leaderboard(nullptr, 10), exception);
}

// тест - создание нового снаряда с пустым указателем на тип снаряда
BOOST_AUTO_TEST_CASE(test_case2) {
    BOOST_CHECK_THROW (new Projectile(nullptr, 0, 0, 0.0), exception);
}

// тест - проверка на корректную загрузку значений очков из файла таблицы рекордов
BOOST_AUTO_TEST_CASE(test_case3) {
	lb = new Leaderboard("D:\\ShootEmUp unit testing\\Debug\\records.ini", 8);
	lb->Update();
	int values[8];
	for (int i = 0; i < 8; i++)
		values[i] = lb->StatValue[i][0];
	int expected[8] = {1000, 950, 500, 450, 400, 350, 250, 100};

	BOOST_CHECK_EQUAL_COLLECTIONS(values, values+8, expected, expected+8);
}

// тест - проверка на корректную загрузку значения жизней из файла таблицы рекордов
BOOST_AUTO_TEST_CASE(test_case4) {
	lb = new Leaderboard("D:\\ShootEmUp unit testing\\Debug\\records.ini", 8);
	lb->Update();
	int values[8];
	for (int i = 0; i < 8; i++)
		values[i] = lb->StatValue[i][1];
	int expected[8] = {3, 2, 0, 1, 2, 3, 2, 1};

	BOOST_CHECK_EQUAL_COLLECTIONS(values, values+8, expected, expected+8);
}

// тест - проверка функции полёта снаряда (посередине экрана, проверка на успешное изменение координаты)
BOOST_AUTO_TEST_CASE(test_case5) {

	prjt = new ProjectileType(1, 6.25, 0.4, 0, 7, 1);
	prj = new Projectile(prjt, 400, 300, 0.0);
	BOOST_REQUIRE_EQUAL(true, prj->Fly());
	BOOST_REQUIRE_LT(400, prj->x);
}

// тест - проверка функции полёта снаряда (около границы экрана, проверка на смерть снаряда при пересечении)
BOOST_AUTO_TEST_CASE(test_case6) {

	prjt = new ProjectileType(1, 6.25, 0.4, 0, 7, 1);
	prj = new Projectile(prjt, 639, 479, 0.0);
	BOOST_REQUIRE_EQUAL(false, prj->Fly());
	BOOST_REQUIRE_EQUAL(0, prj->Health);
}

// тест - проверка на ускорение юнита при перемещении в направлении
BOOST_AUTO_TEST_CASE(test_case7) {

	body = new Unit(0, 0, 300, 340, 5, 0);
	body->DirectionalAccel(false, true, true, false);
	BOOST_REQUIRE_LT(0, body->xVelocity);
	BOOST_REQUIRE_GT(0, body->yVelocity);
}

// тест - проверка на торможение (отсутствие перемещений в направлении)
BOOST_AUTO_TEST_CASE(test_case8) {

	body = new Unit(0, 0, 300, 340, 5, 0);
	body->xVelocity = body->DecelRate;
	body->DirectionalAccel(false, false, false, false);
	BOOST_REQUIRE_GT(body->DecelRate, body->xVelocity);
}
//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()