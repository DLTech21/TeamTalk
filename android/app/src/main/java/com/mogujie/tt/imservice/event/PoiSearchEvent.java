package com.mogujie.tt.imservice.event;

import com.baidu.mapapi.search.core.PoiInfo;

public class PoiSearchEvent {
    public PoiInfo p;
    
    public PoiSearchEvent(PoiInfo data) {
        this.p = data;
    }
}
