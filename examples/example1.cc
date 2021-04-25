#include <cassert>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <raft>
#include <raftio>
#include <raftrandom>
#include <raftmath>
#include <raftutility>

using namespace raft;

template< typename T  > class Sum : public raft::kernel
{
public:
   Sum() : raft::kernel()
   {
      /** declare ports **/
      input.template addPort< T >( "input_a", "input_b" );
      output.template addPort< T  >( "sum" );
   }
   
   virtual raft::kstatus run()
   {
      T a,b;
      input[ "input_a" ].pop( a );
      input[ "input_b" ].pop( b );
      /** smart obj allocate directly on output port **/
      auto out( output[ "sum" ].template allocate_s< T >() );
      /** like an iterator, dereference the out to write to it **/
      (*out) = a + b;
      /** out will automatically release to the next kernel on scope exit **/
      return( raft::proceed );
   }

};


int
main()
{
   const auto count( 1000 );
   /** define type for streams **/
   using type_t = std::int64_t;
   /** 
    * random_variate is a threaded wrapper around the C++ random number gen **/
   using gen = raft::random_variate< std::default_random_engine,
                               std::uniform_int_distribution,
                               type_t >;
   /** this will be our worker 'sum' kernel **/
   using sum    = Sum< type_t >;
   /** make shorter name for print kernel **/
   using p_out  = raft::print< type_t, '\n' >;

   gen a( count ), b( count );
   sum s;
   p_out p;
   raft::map m;
   /** link the only output port of a to the "input_a" port of s **/
   m += a >> s[ "input_a" ];
   /** link the only output port of b to the "input_b" port of s **/
   m += b >> s[ "input_b" ];
   /** take the only output port of s and link it to the only input port of p **/
   m += s >> p;
   /** 
    * NOTE: this will be going away soon, 
    * to be called on scope exit, an explicit
    * barrier call will enable integration with
    * sequential code.
    */
   m.exe();
   return( EXIT_SUCCESS );
}