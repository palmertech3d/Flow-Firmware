/**
 * @Author: Nick Steele
 * @Date:   22:37 Oct 06 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 9:06 Oct 07 2021
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

void TabularDelegator_t::registerTabular(Tabular_t *tabular_ptr, TabularSource_t src){
  if (src >= _TABULAR_SOURCES_COUNT || this->registered_tabulars[src] != NULL) {
    LOG_ERROR("Tabular encountered either a repeated or a bad source number.\n");
    LOG_DEBUG("Info for above error");
    LOG_DEBUG("; src="); LOG_DEBUG(src);
    LOG_DEBUG("; (registered_tabulars[src] == NULL)="); LOG_DEBUG((registered_tabulars[src] == NULL));
    LOG_DEBUG("; _TABULAR_SOURCES_COUNT="); LOG_DEBUG(_TABULAR_SOURCES_COUNT);
    LOG_DEBUG("\n");
    return;
  }

  this->registered_tabulars[src] = tabular_ptr;
} // TabularDelegator_t::registerTabular
