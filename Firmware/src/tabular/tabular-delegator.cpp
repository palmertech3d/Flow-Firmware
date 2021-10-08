/**
 * @Author: Nick Steele
 * @Date:   22:37 Oct 06 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 18:20 Oct 08 2021
 */

#include "tabular/tabular.h"

TabularDelegator_t::TabularDelegator_t() {
  for (uint8_t i = 0; i < _TABULAR_SOURCES_COUNT; i++) {
    this->registered_tabulars[i] = NULL;
  }
}

void TabularDelegator_t::idle(){
  for (uint8_t i = 0; i < _TABULAR_SOURCES_COUNT; i++) {
    if (this->registered_tabulars[i] != NULL)
      this->registered_tabulars[i]->logIfNeeded();
  }
} // TabularDelegator_t::idle

void TabularDelegator_t::unregister(Tabular_t *tabular_ptr, TabularSource_t src){
  uint8_t idx = (uint8_t)src - 1;
  if (idx >= _TABULAR_SOURCES_COUNT || this->registered_tabulars[idx] != tabular_ptr) {
    LOG_ERROR("Tabular attempted to unregister incorrectly.\n");
    LOG_DEBUG(F("Info for above error"));
    LOG_DEBUG(F("; src=")); LOG_DEBUG(src);
    LOG_DEBUG(F("; (registered_tabulars[src])=")); LOG_DEBUG_PTR_ADDR(registered_tabulars[idx]);
    LOG_DEBUG(F("; tabular_ptr=")); LOG_DEBUG_PTR_ADDR(tabular_ptr);
    LOG_DEBUG('\n');
  }
  this->registered_tabulars[idx] = NULL;
} // TabularDelegator_t::idle

void TabularDelegator_t::registerTabular(Tabular_t *tabular_ptr, TabularSource_t src){
  uint8_t idx = (uint8_t)src - 1;
  if (idx >= _TABULAR_SOURCES_COUNT || this->registered_tabulars[idx] != NULL) {
    LOG_ERROR("Tabular encountered either a repeated or a bad source number.\n");
    LOG_DEBUG(F("Info for above error"));
    LOG_DEBUG(F("; src=")); LOG_DEBUG(src);
    LOG_DEBUG(F("; (registered_tabulars[src] == NULL)=")); LOG_DEBUG((registered_tabulars[idx] == NULL));
    LOG_DEBUG(F("; _TABULAR_SOURCES_COUNT=")); LOG_DEBUG(_TABULAR_SOURCES_COUNT);
    LOG_DEBUG('\n');
    return;
  }

  this->registered_tabulars[idx] = tabular_ptr;
} // TabularDelegator_t::registerTabular
