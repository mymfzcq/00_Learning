#ifndef PMU__Performance__HPP
#define PMU__Performance__HPP

#include "pmu1_Statistics.hpp"
#include "PMU_ci.h"
#include "PMU_ct.h"

extern "C" 
{
#include "grlc_c1.h"
#include <math.h>
#include <string.h>
}

namespace PMU
{

	const float PMU_FLT_EPSILON = 1.1920929e-7f;
	//class PerformanceCPUJCP2011;
	class Performance
	{
	public:
		//default constructor
		Performance(void)
		{
		} 

		~Performance(void)
		{
		}


	    virtual void vOutPut(void) = 0;
		Statistics* m_poStatistics;

	protected:
		inline Statistics* vGetMeasurementObject(void)
		{
			return m_poStatistics;
		}

	private:
		
		/**
		* The copy constructor shouldn't be used, therefore it is declared but not defined
		* @param oPerf: Object to be copied
		*/
		Performance( const Performance& oPerf );


	};
	/** *  CLASS END */

}	// End of namespace


#endif //end of PMU__Performance__HPP