class Click
  include DataMapper::Resource

  belongs_to :host
  belongs_to :affiliate
  belongs_to :advertiser
  belongs_to :ad

  property :id,         Serial
  property :ad_id,    Integer
  property :affiliate_id,      Integer
  property :advertiser_id,  Integer
  property :host_id,  Integer
  property :cost, Integer
  property :paid, Boolean, :default => false
  property :created_at, DateTime, :default => lambda { DateTime.now }

  # cost is stored in cents
  def cost_in_dollars
    self.cost / 100.to_f
  end

end
