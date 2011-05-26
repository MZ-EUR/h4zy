class Adgroup
  include DataMapper::Resource

  property :id,         Serial
  property :adtype_id,  Integer
  property :campaign_id, Integer
  property :user_id,    Integer
  property :title,      String
  property :created_at, DateTime, :default => lambda { DateTime.now }
end
