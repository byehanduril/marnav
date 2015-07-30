#include <gtest/gtest.h>
#include <marnav/nmea/dsc.hpp>
#include <marnav/nmea/nmea.hpp>

namespace
{

using namespace marnav;

class Test_nmea_dsc : public ::testing::Test
{
};

TEST_F(Test_nmea_dsc, contruction) { nmea::dsc dsc; }

TEST_F(Test_nmea_dsc, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(nmea::dsc::parse("@@", {10, "@"}));
	EXPECT_ANY_THROW(nmea::dsc::parse("@@", {12, "@"}));
}

TEST_F(Test_nmea_dsc, parse)
{
	static const std::vector<std::string> TESTS
		= {"$CDDSC,20,3380210040,00,21,26,1394807410,2231,,,B,E*75",
			"$CDDSC,20,3380210040,00,21,26,1394807410,2242,,,B,E*71",
			"$CDDSC,20,3664251410,00,21,26,1335111802,0004,,,B,E*72",
			"$CDDSC,20,3380210040,00,21,26,1394807410,0021,,,B,E*74"};

	for (const auto & test : TESTS) {
		auto s = nmea::make_sentence(test);
		EXPECT_NE(nullptr, s);
		if (s) {
			auto dsc = nmea::sentence_cast<nmea::dsc>(s);
			EXPECT_NE(nullptr, dsc);
		}
	}
}

TEST_F(Test_nmea_dsc, get_fmt_spec)
{
	{
		auto s = nmea::sentence_cast<nmea::dsc>(
			nmea::make_sentence("$CDDSC,02,3380210040,00,21,26,1394807410,2231,,,B,E*75"));
		ASSERT_NE(nullptr, s);
		EXPECT_EQ(nmea::dsc::format_specifier::geographical_area, s->get_fmt_spec());
	}
	{
		auto s = nmea::sentence_cast<nmea::dsc>(
			nmea::make_sentence("$CDDSC,12,3380210040,00,21,26,1394807410,2231,,,B,E*74"));
		ASSERT_NE(nullptr, s);
		EXPECT_EQ(nmea::dsc::format_specifier::distress, s->get_fmt_spec());
	}
	{
		auto s = nmea::sentence_cast<nmea::dsc>(
			nmea::make_sentence("$CDDSC,16,3380210040,00,21,26,1394807410,2231,,,B,E*70"));
		ASSERT_NE(nullptr, s);
		EXPECT_EQ(nmea::dsc::format_specifier::all_ships, s->get_fmt_spec());
	}
	{
		auto s = nmea::sentence_cast<nmea::dsc>(
			nmea::make_sentence("$CDDSC,20,3380210040,00,21,26,1394807410,2231,,,B,E*75"));
		ASSERT_NE(nullptr, s);
		EXPECT_EQ(nmea::dsc::format_specifier::individual_station, s->get_fmt_spec());
	}
}

TEST_F(Test_nmea_dsc, get_cat)
{
	{
		auto s = nmea::sentence_cast<nmea::dsc>(
			nmea::make_sentence("$CDDSC,02,3380210040,00,21,26,1394807410,2231,,,B,E*75"));
		ASSERT_NE(nullptr, s);
		EXPECT_EQ(nmea::dsc::category::routine, s->get_cat());
	}
	{
		auto s = nmea::sentence_cast<nmea::dsc>(
			nmea::make_sentence("$CDDSC,02,3380210040,08,21,26,1394807410,2231,,,B,E*7D"));
		ASSERT_NE(nullptr, s);
		EXPECT_EQ(nmea::dsc::category::safety, s->get_cat());
	}
	{
		auto s = nmea::sentence_cast<nmea::dsc>(
			nmea::make_sentence("$CDDSC,02,3380210040,10,21,26,1394807410,2231,,,B,E*74"));
		ASSERT_NE(nullptr, s);
		EXPECT_EQ(nmea::dsc::category::urgency, s->get_cat());
	}
	{
		auto s = nmea::sentence_cast<nmea::dsc>(
			nmea::make_sentence("$CDDSC,02,3380210040,12,21,26,1394807410,2231,,,B,E*76"));
		ASSERT_NE(nullptr, s);
		EXPECT_EQ(nmea::dsc::category::distress, s->get_cat());
	}
}

TEST_F(Test_nmea_dsc, get_mmsi)
{
	auto s = nmea::sentence_cast<nmea::dsc>(
		nmea::make_sentence("$CDDSC,02,3380210040,00,21,26,1394807410,2231,,,B,E*75"));
	ASSERT_NE(nullptr, s);
	EXPECT_EQ(utils::mmsi{338021004}, s->get_mmsi());
}
}