/*
   class_handle.hpp
   
   A C++ class interface for Matlab/Octave
 
   Copyright (c) 2012, Oliver Woodford
   Copyright (c) 2014, Richard Crozier
   All rights reserved.

   Redistribution and use in source and binary forms, with or without 
   modification, are permitted provided that the following conditions are 
   met:

       * Redistributions of source code must retain the above copyright 
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright 
         notice, this list of conditions and the following disclaimer in 
         the documentation and/or other materials provided with the distribution

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
   POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef __CLASS_HANDLE_HPP__
#define __CLASS_HANDLE_HPP__
#include "mex.h"
#include <stdint.h>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <typeinfo>

// define a signature to recognise the class at runtime, ideally
// you should define this before #including this header in the mex function
// file with  unique number for the class
#ifndef CLASS_HANDLE_SIGNATURE
#define CLASS_HANDLE_SIGNATURE 0xFF00F0A5
#endif

template<class base> class class_handle
{
public:
    
    class_handle(base *ptr) : ptr_m(ptr), name_m(typeid(base).name()) 
    { 
        signature_m = CLASS_HANDLE_SIGNATURE; 
    }
    
    ~class_handle() 
    { 
        signature_m = 0; 
        delete ptr_m; 
    }
    
    // function for checking if the wrapped object is still valid
    bool isValid() 
    { 
        return ( (signature_m == CLASS_HANDLE_SIGNATURE) 
                 && !strcmp (name_m.c_str(), typeid (base).name()) ); 
    }
    
    base *ptr() { return ptr_m; }

private:
    
    uint32_t signature_m;
    std::string name_m;
    base *ptr_m;
    
};

template<class base> inline mxArray *convertPtr2Mat(base *ptr)
{
    // lock the memory used in this function so it is not automatically 
    // cleaned up by matlab/octave
    mexLock();
    // create a 64 bit integer array to return a pointer to the class for
    // storage in a normal matlab variable
    mxArray *out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
    // now create a new instance of a class_handle class wrapping the c++ 
    // class to be wrapped, convert the pointer to the class to a uint64 
    // and place it in the array created to hold it
    *((uint64_t *)mxGetData(out)) = reinterpret_cast<uint64_t>(new class_handle<base>(ptr));

    return out;
}

template<class base> inline class_handle<base> *convertMat2HandlePtr(const mxArray *in)
{
    if (mxGetNumberOfElements(in) != 1 || mxGetClassID(in) != mxUINT64_CLASS || mxIsComplex(in))
    {
        mexErrMsgTxt("Input must be a real uint64 scalar.");
    }
    
    class_handle<base> *ptr = reinterpret_cast<class_handle<base> *>(*((uint64_t *)mxGetData(in)));
    
    if (!ptr->isValid())
    {
        mexErrMsgTxt("Handle not valid.");
    }
    
    return ptr;
}

template<class base> inline base *convertMat2Ptr(const mxArray *in)
{
    return convertMat2HandlePtr<base>(in)->ptr();
}

template<class base> inline void destroyObject(const mxArray *in)
{
    // delete the object pointed to by the pointer
    delete convertMat2HandlePtr<base>(in);
    
    // unlock the memory so matlab is free to clean up
    mexUnlock();
}

///////////////////        HELPER MACROS        ///////////////////
//
// The following macros allow easy creation of a map to the wrapped class
// methods. You must first create a wrapper class for the c++ class to which you
// are interfacing. Every method of this interface class which will be called by
// the mex interface must have the following signature:
//
// void methodname (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
//
// When called, the method will be passed all the input arguments passed to the 
// mexfunction.
//
// Then in your mexfunction, use the macros: BEGIN_MEX_CLASS_WRAPPER, 
// REGISTER_CLASS_METHOD and END_MEX_CLASS_WRAPPER to register the methods
// and create the interface function like so:
//
// void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
// {
//     BEGIN_MEX_CLASS_WRAPPER(interfaceClassName)
//       REGISTER_CLASS_METHOD(interfaceClassName,method1name)
//       REGISTER_CLASS_METHOD(interfaceClassName,method2name)
//     BEGIN_MEX_CLASS_WRAPPER(interfaceClassName)
// }
//
// For information, the instance of the wrapped class will then be named 
// interfaceClassName_instance where interfaceClassName should be the name of 
// the class which you previously will have passed into the 
// BEGIN_MEX_CLASS_WRAPPER macro
//
//

// BEGIN_MEX_CLASS_WRAPPER
#define BEGIN_MEX_CLASS_WRAPPER(WRAPPEDCLASS)                                                                \
    typedef void(WRAPPEDCLASS::*classMethod)(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);    \
                                                                                                             \
    std::map<std::string, classMethod> s_map_mex_wrapped_ClassMethodStrs;                                    \

// REGISTER_CLASS_METHOD 
#define REGISTER_CLASS_METHOD(WRAPPEDCLASS,METHOD)  s_map_mex_wrapped_ClassMethodStrs[#METHOD] = &WRAPPEDCLASS::METHOD;

// END_MEX_CLASS_WRAPPER 
#define END_MEX_CLASS_WRAPPER(WRAPPEDCLASS)                                                                  \
    char mex_wrapped_class_cmd_str[1024];                                                                    \
                                                                                                             \
    if (nrhs < 1 || mxGetString(prhs[0], mex_wrapped_class_cmd_str, sizeof(mex_wrapped_class_cmd_str)))      \
    {                                                                                                        \
        mexErrMsgTxt("First input should be a command string less than 128 characters long.");               \
    }                                                                                                        \
                                                                                                             \
                                                                                                             \
    if (!strcmp("new", mex_wrapped_class_cmd_str))                                                           \
    {                                                                                                        \
                                                                                                             \
        if (nlhs != 1)                                                                                       \
            mexErrMsgTxt("New: One output expected.");                                                       \
                                                                                                             \
        plhs[0] = convertPtr2Mat<WRAPPEDCLASS>(new WRAPPEDCLASS);                                            \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
                                                                                                             \
    if (nrhs < 2)                                                                                            \
    {                                                                                                        \
        mexErrMsgTxt("Second input should be a class instance handle.");                                     \
    }                                                                                                        \
                                                                                                             \
                                                                                                             \
    if (!strcmp("delete", mex_wrapped_class_cmd_str))                                                        \
    {                                                                                                        \
                                                                                                             \
        destroyObject<WRAPPEDCLASS>(prhs[1]);                                                                \
                                                                                                             \
        if (nlhs != 0 || nrhs != 2)                                                                          \
            mexWarnMsgTxt("Delete: Unexpected arguments ignored.");                                          \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
                                                                                                             \
    WRAPPEDCLASS* WRAPPEDCLASS ## _instance = convertMat2Ptr<WRAPPEDCLASS>(prhs[1]);                         \
                                                                                                             \
                                                                                                             \
    if (s_map_mex_wrapped_ClassMethodStrs.count(mex_wrapped_class_cmd_str) > 0)                              \
    {                                                                                                        \
        classMethod theMethodpntr = s_map_mex_wrapped_ClassMethodStrs[mex_wrapped_class_cmd_str];            \
                                                                                                             \
        (WRAPPEDCLASS ## _instance->*theMethodpntr)(nlhs, plhs, nrhs, prhs);                                 \
    }                                                                                                        \
    else                                                                                                     \
    {                                                                                                        \
        mexErrMsgTxt("Unrecognised class command string.");                                                  \
    }                                                                                                        \
    

///////////////////        HELPER FUNCTIONS        ///////////////////

// mex helper functions
namespace mexutils {

  
void mxtestnumeric (const mxArray* testMxArray) {
 
   if (!mxIsNumeric(testMxArray))
   {
     mexErrMsgIdAndTxt("CPP:mxtestnumeric",
         "Input argument is not numeric.");
   } 
   
}

// check the number of input arguments provided
int mxnarginchk (int nargs, std::vector<int> nallowed, int offset=0)
{
  int offsetnargs = nargs-offset;
  
  if (nallowed.size () > 0)
  {
     for (int i = 0; i < nallowed.size (); i++)
     {
         if (nallowed[i] == offsetnargs)
         {
             // return as we have a matching number of arguments
             return offsetnargs;
         }
     }
  }
  else
  {
      mexErrMsgIdAndTxt("CPP:mxnarginchk",
           "No allowed number of arguments supplied.");
  }
  
  mexErrMsgIdAndTxt("CPP:mxnarginchk",
         "Incorrect number of input arguments. You supplied %i args with an offset of %i", nargs, offset);
  
  return offsetnargs;
}

void mxnaroutgchk (const int nlhs, int ntharg)
{
  
  if (ntharg <= nlhs)
  {
      // return as we have a matching number of arguments
      return;
  }
  
  // throw an error
  mexErrMsgIdAndTxt("CPP:mxnargoutchk",
         "Incorrect number of output arguments.");
  
  return;
}

// Get the n'th scalar input argumetn to a mexfunction
double mxnthargscalar (int nrhs, const mxArray *prhs[], int ntharg, int offset=0)
{
  
   ntharg = ntharg + offset;
  
   if (ntharg > nrhs)
   {
     mexErrMsgIdAndTxt("CPP:mxnthargscalar",
         "Requested argument is greater than total number of arguments.");
   }
   
   // check matrix is numeric
   mxtestnumeric (prhs[ntharg-1]); 
   
   if ((mxGetN(prhs[ntharg-1]) != 1) || (mxGetM(prhs[ntharg-1]) != 1))
   {
     mexErrMsgIdAndTxt("CPP:mxnthargscalar",
         "Input argument is not scalar.");
   }
   
   return mxGetScalar(prhs[ntharg-1]);
   
}


// return array of integers
void mxSetLHS (const int* const out, int argn, int size, const int nlhs, mxArray* plhs[])
{

    // check the argument position is possible
    mxnaroutgchk (nlhs, argn);
  
    // create the output matrix to hold the vector of numbers
    plhs[argn-1] = mxCreateNumericMatrix(1, size, mxINT32_CLASS, mxREAL);
    
    int * outArray = (int *) mxGetData(plhs[argn-1]);

    if (outArray)
    {
        // copy the data
        for (int i = 0; i < size; i++)
        {
            outArray[i] = *(out+i);
        }
    }
    else
    {
        mexErrMsgIdAndTxt("CPP:mxSetLHS",
         "Unable to set output.");
    }
}

// return integer
void mxSetLHS (const int out, int argn, const int nlhs, mxArray* plhs[])
{
    //int outcp = out;
  
    // call the function for returning a vector, with a pointer to the the 
    // output data
    mxSetLHS (&out, argn, 1, nlhs, plhs);
}


// return std::vector of integers
void mxSetLHS (const std::vector<int> out, int argn, const int nlhs, mxArray* plhs[])
{

    // check the argument position is possible
    mxnaroutgchk (nlhs, argn);
  
    // create the output matrix to hold the vector of numbers
    plhs[argn-1] = mxCreateNumericMatrix(1, out.size (), mxINT32_CLASS, mxREAL);
    
    int * outArray = (int *) mxGetData(plhs[argn-1]);

    if (outArray)
    {
        // copy the data
        for (int i = 0; i < out.size (); i++)
        {
            outArray[i] = out[i];
        }
    }
    else
    {
        mexErrMsgIdAndTxt("CPP:mxSetLHS",
         "Unable to set output.");
    }
}


// return array of floats
void mxSetLHS (const float* const out, int argn, int size, const int nlhs, mxArray* plhs[])
{
    // check the argument position is possible
    mxnaroutgchk (nlhs, argn);
  
    // create the output matrix to hold the vector of numbers
    plhs[argn-1] = mxCreateNumericMatrix(1, size, mxSINGLE_CLASS, mxREAL);
    
    float * outArray = (float *) mxGetData(plhs[argn-1]);

    if (outArray)
    {
        // copy the data
        for (int i = 0; i < size; i++)
        {
            outArray[i] = *(out+i);
        }
    }
    else
    {
        mexErrMsgIdAndTxt("CPP:mxSetLHS",
         "Unable to set output.");
    }
    
}

// return float
void mxSetLHS (const float out, int argn, const int nlhs, mxArray* plhs[])
{
    // call the function for returning a vector, with a pointer to the the 
    // output data
    mxSetLHS (&out, argn, 1, nlhs, plhs);
}

// return std::vector of floats
void mxSetLHS (const std::vector<float> out, int argn, const int nlhs, mxArray* plhs[])
{

    // check the argument position is possible
    mxnaroutgchk (nlhs, argn);
  
    // create the output matrix to hold the vector of numbers
    plhs[argn-1] = mxCreateNumericMatrix(1, out.size (), mxSINGLE_CLASS, mxREAL);
    
    float * outArray = (float *) mxGetData(plhs[argn-1]);

    if (outArray)
    {
        // copy the data
        for (int i = 0; i < out.size (); i++)
        {
            outArray[i] = out[i];
        }
    }
    else
    {
        mexErrMsgIdAndTxt("CPP:mxSetLHS",
         "Unable to set output.");
    }
}


// return array of doubles
void mxSetLHS (const double* const out, int argn, int size, const int nlhs, mxArray* plhs[])
{
    // check the argument position is possible
    mxnaroutgchk (nlhs, argn);
  
    // create the output matrix to hold the vector of numbers
    plhs[argn-1] = mxCreateNumericMatrix(1, size, mxDOUBLE_CLASS, mxREAL);
    
    double * outArray = (double *) mxGetData(plhs[argn-1]);

    if (outArray)
    {
        // copy the data
        for (int i = 0; i < size; i++)
        {
            outArray[i] = *(out+i);
        }
    }
    else
    {
        mexErrMsgIdAndTxt("CPP:mxSetLHS",
         "Unable to set output.");
    }
}

// return double
void mxSetLHS (const double out, int argn, const int nlhs, mxArray* plhs[])
{
    mxSetLHS (&out, argn, 1, nlhs, plhs);
}

// return std::vector of doubles
void mxSetLHS (const std::vector<double> out, int argn, const int nlhs, mxArray* plhs[])
{

    // check the argument position is possible
    mxnaroutgchk (nlhs, argn);
  
    // create the output matrix to hold the vector of numbers
    plhs[argn-1] = mxCreateNumericMatrix(1, out.size (), mxDOUBLE_CLASS, mxREAL);
    
    double * outArray = (double *) mxGetData(plhs[argn-1]);

    if (outArray)
    {
        // copy the data
        for (int i = 0; i < out.size (); i++)
        {
            outArray[i] = out[i];
        }
    }
    else
    {
        mexErrMsgIdAndTxt("CPP:mxSetLHS",
         "Unable to set output.");
    }
}


// wrapper class for mwArray, aminly to ease indexing
class mxNumericArrayWrapper
{
public:
  
  // constructor
  mxNumericArrayWrapper (const mxArray* wrappedMxArray)
  {
      mxtestnumeric (wrappedMxArray);
    
      wMxArray = wrappedMxArray;
    
      // get the number of dimensions
      mwSize ndims = mxGetNumberOfDimensions(wMxArray);
    
      mwSize* dimspntr = mxGetDimensions(wMxArray);
    
      // get the dimensions and push them into the _dimensions vector
      for (int i = 0; i < ndims; i++)
      {
          mwSize dimsize = *(dimspntr+i);
          _dimensions.push_back(dimsize);
      }
      
      
      
      
  }
  
  double getDoubleValue (std::vector<mwSize> index)
  {
      // check it's  double matrix
      if (!mxIsDouble (wMxArray))
      {
          mexErrMsgIdAndTxt("CPP:mxArrayWrapper:notdouble",
              "Double value requested for non-double matrix.");          
      }
    
      // check dimensions are within range
      checkDimensions (index);
      
      // make an array of the appropriate size to hold the indices
      mwIndex* subs = new mwIndex[_dimensions.size ()];
      // copy the index into the subs array
      for (int i=0; i < index.size (); i++) { *(subs+i) = index[i]; }
      // get the linear index into the underlying data array
      mwIndex linindex = mxCalcSingleSubscript(wMxArray, (mwSize)(_dimensions.size ()), subs);
      // delete the memory allocated for the index
      delete[] subs;
      
      // get the data from the array
      double* data = mxGetPr(wMxArray);
      
      return data[(int)linindex];
      
  }
  
  void checkDimensions (const std::vector<mwSize> &index)
  {
      if (index.size () != _dimensions.size ())
      {
          mexErrMsgIdAndTxt("CPP:mxArrayWrapper:invalidindex",
              "Wrong number of dimensions specified.");
      }
      
      
      for (int i=0; i < index.size (); i++)
      {
          // check we are not outwith any dimensions
          if (index[i] > _dimensions[i])
          {
              mexErrMsgIdAndTxt("CPP:mxArrayWrapper:invalidindex",
                  "Index to dimension %i out of bounds, value %i out of bound %i.", 
                  i+1, index[i], _dimensions[i] );
          }
      }
  }
  
  std::vector<mwSize> getDimensions ()
  {
      return _dimensions;
  }
  
  mwSize getRows ()
  {
      return _dimensions[0];
  }
  
  mwSize getColumns ()
  {
      return _dimensions[1];
  }
  
private:
  
  const mxArray* wMxArray;
  std::vector<mwSize> _dimensions;
  
};
 


} // namespace mexutils

#endif // __CLASS_HANDLE_HPP__
