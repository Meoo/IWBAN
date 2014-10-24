
print "MAP LOADING BEGIN"

-- Event catcher test
function MAP:test()
    print "pouet"
end

save.test = (save.test or 0) + 1
save.string = "test " .. save.test .. " !"
print (save.string)

print "MAP LOADING END"
