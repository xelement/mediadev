#ifndef HEVC_PARSER_IMPL
#define HEVC_PARSER_IMPL



#include "HevcParser.h"
#include "BitstreamReader.h"

#include <map>
#include <list>
#include <memory>

namespace HEVC
{
  class HevcParserImpl: public Parser
  {
    public:
      virtual std::size_t process(const uint8_t *pdata, std::size_t size, std::size_t offset = 0);

      virtual void addConsumer(Consumer *pconsumer);
      virtual void releaseConsumer(Consumer *pconsumer);

    protected:
      void processNALUnit(const uint8_t *pdata, std::size_t size, const Parser::Info &info);
      NALUnitType processNALUnitHeader(BitstreamReader &bs);
      void processVPS(std::shared_ptr<VPS> pvps, BitstreamReader &bs, const Parser::Info &info);
      void processSPS(std::shared_ptr<SPS> psps, BitstreamReader &bs, const Parser::Info &info);
      void processPPS(std::shared_ptr<PPS> ppps, BitstreamReader &bs, const Parser::Info &info);
      void processSlice(std::shared_ptr<Slice> pslice, BitstreamReader &bs, const Parser::Info &info);
      void processAUD(std::shared_ptr<AUD> paud, BitstreamReader &bs, const Parser::Info &info);
      void processSEI(std::shared_ptr<SEI> psei, BitstreamReader &bs, const Parser::Info &info);
      void processSliceHeader(std::shared_ptr<Slice> pslice, BitstreamReader &bs, const Parser::Info &info);
      void processSliceData(std::shared_ptr<Slice> pslice, BitstreamReader &bs, const Parser::Info &info);
      ProfileTierLevel processProfileTierLevel(std::size_t max_sub_layers_minus1, BitstreamReader &bs, const Parser::Info &info);
      HrdParameters processHrdParameters(uint8_t commonInfPresentFlag, std::size_t maxNumSubLayersMinus1, BitstreamReader &bs);
      SubLayerHrdParameters processSubLayerHrdParameters(uint8_t sub_pic_hrd_params_present_flag, std::size_t CpbCnt, BitstreamReader &bs);
      ShortTermRefPicSet processShortTermRefPicSet(std::size_t stRpsIdx, size_t num_short_term_ref_pic_sets, const std::vector<ShortTermRefPicSet> &refPicSets, std::shared_ptr<SPS> psps, BitstreamReader &bs, const Parser::Info &info);
      VuiParameters processVuiParameters(std::size_t sps_max_sub_layers_minus1, BitstreamReader &bs);
      ScalingListData processScalingListData(BitstreamReader &bs);
      RefPicListModification processRefPicListModification(BitstreamReader &bs, std::shared_ptr<Slice> pslice);
      PredWeightTable processPredWeightTable(BitstreamReader &bs, std::shared_ptr<Slice> pslice);

      void onWarning(const std::string &warning, const Info *pInfo, WarningType type);

      std::map<uint32_t, std::shared_ptr<VPS> >          m_vpsMap;
      std::map<uint32_t, std::shared_ptr<SPS> >          m_spsMap;
      std::map<uint32_t, std::shared_ptr<PPS> >          m_ppsMap;

      std::list<Consumer *>          m_consumers;
  };
}

#endif
