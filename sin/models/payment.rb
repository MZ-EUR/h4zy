class Payment
  include DataMapper::Resource

  belongs_to :advertiser
  belongs_to :user

  property :id,             Serial
  property :user_id,        Integer
  property :advertiser_id,  Integer
  property :amount,         Integer
  property :created_at,     DateTime, :default => lambda { DateTime.now }
end
