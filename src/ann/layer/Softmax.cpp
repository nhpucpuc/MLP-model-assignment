/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Softmax.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"
#include "ann/functions.h"
#include "sformat/fmt_lib.h"
#include <filesystem> //require C++17
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name) : m_nAxis(axis) {
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {}

Softmax::~Softmax() {}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) 
{

  // xt::xarray<double> Xmax = xt::amax(X, {m_nAxis}, xt::keep_dims);

  // xt::xarray<double> expX = xt::exp(X - Xmax);

  // xt::xarray<double> sumExpX = xt::sum(expX, {m_nAxis}, xt::keep_dims);

  // this->m_aCached_Y = expX / sumExpX;
  // return m_aCached_Y;
  m_aCached_Y = softmax(X , m_nAxis);
  return m_aCached_Y;
}

xt::xarray<double> Softmax::backward(xt::xarray<double> DY) 
{
  // Todo CODE YOUR
  // xt::xarray<double> diag_Y = xt::diag(this->m_aCached_Y);

  // xt::xarray<double> outer_Y = xt::linalg::outer(this->m_aCached_Y, this->m_aCached_Y);

  // xt::xarray<double> jacobian_Y = diag_Y - outer_Y;

  // xt::xarray<double> dZ = xt::linalg::dot(jacobian_Y, DY);
  // return dZ;

      if (this->m_aCached_Y.dimension() == 1) 
    {
        xt::xarray<double> diag_Y = xt::diag(this->m_aCached_Y);
        xt::xarray<double> outer_Y = xt::linalg::outer(this->m_aCached_Y, this->m_aCached_Y);
        xt::xarray<double> jaco_Y = diag_Y - outer_Y;
        
        xt::xarray<double> dZ = xt::linalg::dot(jaco_Y, DY);
        return dZ;
    } 
    else 
    {
        xt::xarray<double> dZ = xt::zeros_like(DY);
        
        for (size_t i = 0; i < DY.shape()[0]; ++i) 
        {
            xt::xarray<double> diag_Y = xt::diag(xt::view(this->m_aCached_Y, i, xt::all()));
            xt::xarray<double> outer_Y = xt::linalg::outer(xt::view(this->m_aCached_Y, i, xt::all()), xt::view(this->m_aCached_Y, i, xt::all()));
            xt::xarray<double> jaco_Y = diag_Y - outer_Y;
            
            xt::view(dZ, i, xt::all()) = xt::linalg::dot(jaco_Y, xt::view(DY, i, xt::all()));
        }
        
        return dZ;
    }
}

string Softmax::get_desc() {
  string desc = fmt::format("{:<10s}, {:<15s}: {:4d}", "Softmax",
                            this->getname(), m_nAxis);
  return desc;
}
