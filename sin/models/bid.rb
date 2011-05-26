class Bid
  include DataMapper::Resource

  property :id,         Serial
  property :user_id,    Integer
  property :ad_id,      Integer
  property :auto_bid,   Boolean
  property :amount,     Integer
  property :created_at, DateTime, :default => lambda { DateTime.now }
end
