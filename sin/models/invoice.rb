class Invoice
  include DataMapper::Resource

  belongs_to :advertiser
  belongs_to :user

  property :id,             Serial
  property :user_id,        Integer
  property :advertiser_id,  Integer
  property :amount,         Integer
  property :paid,           Boolean, :default => false
  property :paid_at,        DateTime
  property :created_at,     DateTime, :default => lambda { DateTime.now }
end
