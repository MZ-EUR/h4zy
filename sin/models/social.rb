class Social
  include DataMapper::Resource

  property :id,         Serial
  property :user_id,    Integer
  property :bid_id,     Integer
  property :created_at, DateTime, :default => lambda { DateTime.now }
end
