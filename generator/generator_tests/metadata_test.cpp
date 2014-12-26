#include "../../testing/testing.hpp"
#include "../../base/logging.hpp"

#include "../../indexer/classificator_loader.hpp"
#include "../../generator/osm2meta.hpp"
#include "../../coding/writer.hpp"
#include "../../coding/reader.hpp"

UNIT_TEST(Metadata_ValidateAndFormat_stars)
{
  FeatureParams params;
  MetadataTagProcessor p(params);

  // ignore incorrect values
  p("stars", "0");
  TEST(params.GetMetadata().Empty(), ());
  p("stars", "-1");
  TEST(params.GetMetadata().Empty(), ());
  p("stars", "aasdasdas");
  TEST(params.GetMetadata().Empty(), ());
  p("stars", "8");
  TEST(params.GetMetadata().Empty(), ());
  p("stars", "10");
  TEST(params.GetMetadata().Empty(), ());
  p("stars", "910");
  TEST(params.GetMetadata().Empty(), ());
  p("stars", "100");
  TEST(params.GetMetadata().Empty(), ());

  // check correct values
  p("stars", "1");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "1", ())
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_STARS);

  p("stars", "2");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "2", ())
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_STARS);

  p("stars", "3");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "3", ())
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_STARS);

  p("stars", "4");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "4", ())
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_STARS);

  p("stars", "5");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "5", ())
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_STARS);

  p("stars", "6");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "6", ())
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_STARS);

  p("stars", "7");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "7", ())
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_STARS);

  // check almost correct values
  p("stars", "4+");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "4", ())
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_STARS);

  p("stars", "5s");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "5", ())
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_STARS);

}

UNIT_TEST(Metadata_ValidateAndFormat_operator)
{
  classificator::Load();
  Classificator const & c = classif();
  uint32_t const type_atm = c.GetTypeByPath({ "amenity", "atm" });
  uint32_t const type_fuel = c.GetTypeByPath({ "amenity", "fuel" });

  FeatureParams params;
  MetadataTagProcessor p(params);

  // ignore tag 'operator' if feature have inappropriate type
  p("operator", "Some");
  TEST(params.GetMetadata().Empty(), ());

  params.SetType(type_atm);
  p("operator", "Some");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_OPERATOR), "Some", ());
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_OPERATOR);

  params.SetType(type_fuel);
  p("operator", "Some");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_OPERATOR), "Some", ());
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_OPERATOR);

  params.SetType(type_atm);
  params.AddType(type_fuel);
  p("operator", "Some");
  TEST_EQUAL(params.GetMetadata().Get(feature::FeatureMetadata::FMD_OPERATOR), "Some", ());
  params.GetMetadata().Drop(feature::FeatureMetadata::FMD_OPERATOR);
}

UNIT_TEST(Metadata_ReadWrite_Intermediate)
{
  FeatureParams params;
  FeatureParams params_test;
  MetadataTagProcessor p(params);

  vector<char> buffer;
  MemWriter<vector<char> > writer(buffer);

  p("stars", "3");
  p("phone", "+123456789");
  p("opening_hours", "24/7");
  p("cuisine", "regional");
  p("operator", "Unused");
  params.GetMetadata().Serialize(writer);

  MemReader reader(buffer.data(), buffer.size());
  ReaderSource<MemReader> src(reader);
  params_test.GetMetadata().Deserialize(src);

  TEST_EQUAL(params_test.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "3", ())
  TEST_EQUAL(params_test.GetMetadata().Get(feature::FeatureMetadata::FMD_PHONE_NUMBER), "+123456789", ())
  TEST_EQUAL(params_test.GetMetadata().Get(feature::FeatureMetadata::FMD_OPEN_HOURS), "24/7", ())
  TEST_EQUAL(params_test.GetMetadata().Get(feature::FeatureMetadata::FMD_CUISINE), "regional", ())
  TEST(params_test.GetMetadata().Get(feature::FeatureMetadata::FMD_OPERATOR).empty(), ())
}

UNIT_TEST(Metadata_ReadWrite_MWM)
{
  FeatureParams params;
  FeatureParams params_test;
  MetadataTagProcessor p(params);

  vector<char> buffer;
  MemWriter<vector<char> > writer(buffer);

  p("stars", "3");
  p("phone", "+123456789");
  p("opening_hours", "24/7");
  p("cuisine", "regional");
  p("operator", "Unused");
  params.GetMetadata().SerializeToMWM(writer);

  MemReader reader(buffer.data(), buffer.size());
  ReaderSource<MemReader> src(reader);
  params_test.GetMetadata().DeserializeFromMWM(src);

  TEST_EQUAL(params_test.GetMetadata().Get(feature::FeatureMetadata::FMD_STARS), "3", ())
  TEST_EQUAL(params_test.GetMetadata().Get(feature::FeatureMetadata::FMD_PHONE_NUMBER), "+123456789", ())
  TEST_EQUAL(params_test.GetMetadata().Get(feature::FeatureMetadata::FMD_OPEN_HOURS), "24/7", ())
  TEST_EQUAL(params_test.GetMetadata().Get(feature::FeatureMetadata::FMD_CUISINE), "regional", ())
  TEST(params_test.GetMetadata().Get(feature::FeatureMetadata::FMD_OPERATOR).empty(), ())
}