#include "dse.hpp"
#include <marnav/nmea/checks.hpp>
#include <marnav/nmea/io.hpp>
#include <marnav/utils/unique.hpp>

namespace marnav
{
namespace nmea
{

namespace
{
static dse::query_flag flag_mapping(
	typename std::underlying_type<dse::query_flag>::type value) throw(std::invalid_argument)
{
	switch (value) {
		case 'Q':
			return dse::query_flag::query;
		case 'R':
			return dse::query_flag::reply;
		case 'A':
			return dse::query_flag::a;
	}
	throw std::invalid_argument{"invaild value for conversion to dse::query_flag"};
}

static dse::code_id code_mapping(typename std::underlying_type<dse::code_id>::type value) throw(
	std::invalid_argument)
{
	switch (value) {
		case 0:
			return dse::code_id::enhanced_position_resolution;
		case 1:
			return dse::code_id::source_and_data_of_position;
		case 2:
			return dse::code_id::current_speed_of_vessel;
		case 3:
			return dse::code_id::current_course_of_vessel;
		case 4:
			return dse::code_id::additional_station_identification;
		case 5:
			return dse::code_id::enhanced_geographic_area;
		case 6:
			return dse::code_id::number_of_persons_on_board;
	}
	throw std::invalid_argument{"invaild value for conversion to dse::code_id"};
}
}

std::string to_string(dse::query_flag value) throw(std::invalid_argument)
{
	switch (value) {
		case dse::query_flag::query:
			return "Q";
		case dse::query_flag::reply:
			return "R";
		case dse::query_flag::a:
			return "A";
	}
	throw std::invalid_argument{"invaild value for conversion of dse::query_flag"};
}

std::string to_string(dse::code_id value) throw(std::invalid_argument)
{
	swtich(value)
	{
		case dse::code_id::enhanced_position_resolution:
			return "0";
		case dse::code_id::source_and_data_of_position:
			return "1";
		case dse::code_id::current_speed_of_vessel:
			return "2";
		case dse::code_id::current_course_of_vessel:
			return "3";
		case dse::code_id::additional_station_identification:
			return "4";
		case dse::code_id::enhanced_geographic_area:
			return "5";
		case dse::code_id::number_of_persons_on_board:
			return "6";
	}
	throw std::invalid_argument{"invaild value for conversion of dse::code_id"};
}

constexpr const char * dse::TAG;

dse::dse()
	: sentence(ID, TAG, talker_id::communications_dsc)
	, number_of_messages(1)
	, sentence_number(1)
	, flag(query_flag::query)
	, address(0)
{
}

utils::mmsi dse::get_mmsi() const
{
	return utils::mmsi{static_cast<utils::mmsi::value_type>(address / 10)};
}

void dse::set_mmsi(const utils::mmsi & t)
{
	address = t;
	address *= 10;
}

std::unique_ptr<sentence> dse::parse(const std::string & talker,
	const std::vector<std::string> & fields) throw(std::invalid_argument)
{
	if (fields.size() != 6)
		throw std::invalid_argument{"invalid number of fields in dse::parse"};

	std::unique_ptr<sentence> result = utils::make_unique<dse>();
	result->set_talker(talker);
	dse & detail = static_cast<dse &>(*result);

	read(fields[0], detail.number_of_messages);
	read(fields[1], detail.sentence_number);
	read(fields[2], detail.flag, flag_mapping);
	read(fields[3], detail.address);

	// TODO: read data set fields

	return result;
}

std::vector<std::string> dse::get_data() const
{
	return {to_string(number_of_messages), to_string(sentence_number), to_string(flag),
		format(address, 10), "", ""};
}
}
}
