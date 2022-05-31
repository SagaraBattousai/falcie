
Target
========

.. doxygenclass:: pulse::Target
   :members:
   :undoc-members:



..
   		/*----
  		 * Bitcoin uses theta * 2 ^ (8 * (alpha - 3)),
  		 * where the 8 scales bits to bytes. We use
  		 * theta * 2^(8 * (alpha - 4)) which (I think) gives
  		 * us more small numbers.
  		 *
  		 * Bitcoin checks its difficulty every 2016 blocks
  		 * i.e. 8 * (255 - 3). No idea what the rest of what I
  		 * werote means.
  		 * "every 223 * 8 means calculations are for 256 blocks.
  		 * as" etc
  		 */
  
  		 /// Using an exponential shifter of 8 allows for nice shifting functionality as
  		 /// << and >> are bit shifts so multiplying by 8 allows for byte shifting.
  		 /// Note: The shifting functionality comes from the fact target is calculated by
  		 /// theta * 2 ^ (8 * (alpha - 4)) which is equivilent to theta << (8 * (alpha - 4)).
  		 ///constexpr std::uint32_t CACTUAR_EXP_SCALAR = 8;
  		 /// 
  		 //constexpr std::uint32_t CACTUAR_EXP_SHIFTER = 4;
  		 //constexpr std::uint32_t CACTUAR_EXP_SCALED_SHIFTER = 32;
  
  		 //constexpr std::uint32_t MULTIPLIER_MASK = 0x00FFFFFF;
