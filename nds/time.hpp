#pragma once

#include "gen4-tools/utility/data_types.hpp"

#define NDS_FPS 59.8261

namespace gen4::nds
{
	struct Date
	{
		ui8 year;
		ui8 month;
		ui8 day;
	};

	class Time
	{
	private:
		//Seconds passed since 2000-1-1 00:00:00
		ui32 m_seconds { 0 };

	private:
		//Compute the number of days in the provided month, depending on the provided year
		static const ui8 days_in_month(const ui8& t_year, const ui8& t_month)
		{
			switch (t_month)
			{
			case 2:
				return ((t_year % 4) == 0) ? 29 : 28;
			case 4:
			case 6:
			case 9:
			case 11:
				return 30;
			default:
				return 31;
			}
		}

	public:
		Time(const ui32& t_seconds) : m_seconds(t_seconds) {}

		//The year must be modulo 100
		Time
		(
			const ui8& t_year,
			const ui8& t_month,
			const ui8& t_day,
			const ui8& t_hour,
			const ui8& t_minute,
			const ui8& t_second
		)
		{
			//Compute total seconds from second, minute, hour and day as they are provided
			m_seconds =
			(
				t_second +
				t_minute    * 60 +
				t_hour      * 60 * 60 +
				(t_day - 1) * 60 * 60 * 24
			);

			//Compute total seconds from the provided month
			for (ui8 i_month = 0; i_month < t_month - 1; ++i_month)
			{
				m_seconds += days_in_month(t_year, i_month + 1) * 60 * 60 * 24;
			}

			//Compute total seconds from the provided year
			for (ui8 i_year = 0; i_year < t_year; ++i_year)
			{
				for (ui8 i_month = 0; i_month < 12; ++i_month)
				{
					m_seconds += days_in_month(i_year, i_month + 1) * 60 * 60 * 24;
				}
			}
		}

		//Subtract the provided amount of seconds from the total
		void subtract_seconds(const ui32& t_seconds) 
		{
			m_seconds = static_cast<ui32>((static_cast<si64>(m_seconds) - static_cast<si64>(t_seconds)) % 3155760000U);
		}

		//Compute the date from the total seconds
		const Date get_date() const
		{
			ui8 day = 0;
			ui8 month = 1;
			ui8 year = static_cast<ui8>(static_cast<fp64>(m_seconds) / (60.0 * 60.0 * 24.0 * 365.25));

			Time year_time { year, 1, 1, 0, 0, 0 };
			ui32 time_since_new_year = (m_seconds / (60 * 60 * 24)) - (year_time.get_total_seconds() / (60 * 60 * 24)) + 1;
			for (ui32 i_day = 1; i_day <= time_since_new_year; ++i_day)
			{
				++day;
				if (day == days_in_month(year, month) + 1)
				{
					day = 1;
					++month;
				}
			}

			return { year, month, day };
		}

		const ui8 get_hour() const { return (m_seconds / (60 * 60)) % 24; }
		const ui8 get_minute() const { return (m_seconds / 60) % 60; }
		const ui8 get_second() const { return m_seconds % 60; }

		const ui32& get_total_seconds() const { return m_seconds; }

		//Function used to generate the seed from the current time and delay
		const ui32 generate_rng_seed(const ui32& t_delay)
		{
			Date date = get_date();

			return 
			(
				((date.month * date.day + get_minute() + get_second()) << 24) |
				(get_hour() << 16) + date.year + t_delay
			);
		}

	};
}