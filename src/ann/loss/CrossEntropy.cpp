/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   CrossEntropy.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"
#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction) : ILossLayer(reduction) {}

CrossEntropy::CrossEntropy(const CrossEntropy& orig) : ILossLayer(orig) {}

CrossEntropy::~CrossEntropy() {}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t) 
{
  int Class = (X.dimension() >= 2) ? X.shape()[1] : 1;
  this->m_aCached_Ypred = X;
  
  if ((xt::amin(t)() != 0.0) || (xt::amax(t)() != 1.0)) 
  {
      xt::xarray<unsigned long> t_Class = xt::cast<unsigned long>(t);
      this->m_aYtarget = onehot_enc(t_Class, Class);
  } 
  else 
  {
      if(t.shape()[0] == 2)
      {
        xt::xarray<unsigned long> t_Class = xt::cast<unsigned long>(t);
        this->m_aYtarget = onehot_enc(t_Class, Class);
      }
      this->m_aYtarget = t;
  } 
  return cross_entropy(X, m_aYtarget, this->m_eReduction);
}

xt::xarray<double> CrossEntropy::backward() 
{
  xt::xarray<double> grad = -(this->m_aYtarget / (this->m_aCached_Ypred + 1e-7));

  if (this->m_eReduction == REDUCE_MEAN) 
  {
      grad /= this->m_aCached_Ypred.shape()[0];
  }

  return grad;
}
