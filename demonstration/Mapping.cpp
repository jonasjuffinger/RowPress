#include "Mapping.h"

uintptr_t Mapping::base_address;

void Mapping::decode_new_address(uintptr_t addr)
{
    auto p = addr;
    int res = 0;
    for (unsigned long i : mem_configuration->DRAM_MTX) {
        res <<= 1ULL;
        res |= (int) __builtin_parityl(p & i);
    }
    bank = (res >> mem_configuration->BK_SHIFT) & mem_configuration->BK_MASK;
    row = (res >> mem_configuration->ROW_SHIFT) & mem_configuration->ROW_MASK;
    column = (res >> mem_configuration->COL_SHIFT) & mem_configuration->COL_MASK;
}

int Mapping::linearize() {
  return (this->bank << mem_configuration->BK_SHIFT) 
        | (this->row << mem_configuration->ROW_SHIFT) 
        | (this->column << mem_configuration->COL_SHIFT);
}

uintptr_t Mapping::to_virt() {
  int res = 0;
  int l = this->linearize();
  for (unsigned long i : mem_configuration->ADDR_MTX) {
    res <<= 1ULL;
    res |= (int) __builtin_parityl(l & i);
  }
  return res + this->base_address;
}

void Mapping::increment_row()
{
  this->row++;
}

void Mapping::increment_bank()
{
  this->bank++;
}

void Mapping::decrement_row()
{
  this->row--;
}

void Mapping::increment_column_dw()
{
  this->column+=8;
}

void Mapping::increment_column_cb()
{
  this->column+=64;
}

void Mapping::reset_column()
{
  this->column = 0;
}

int Mapping::get_bank()
{
    return bank;
}

int Mapping::get_row()
{
    return row;
}

int Mapping::get_column()
{
    return column;
}