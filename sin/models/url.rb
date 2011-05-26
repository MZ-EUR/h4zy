class Url
  include DataMapper::Resource
  
  belongs_to :campaign
  belongs_to :user
  belongs_to :bid

  property :id,         Serial
  property :user_id,    Integer
  property :bid_id,     Integer
  property :campaign_id,Integer
  property :url,        String
  property :created_at, DateTime, :default => lambda { DateTime.now }

  validates_uniqueness_of :url, :scope => :campaign_id,
    :message => "Campaign already has this url!"
end
